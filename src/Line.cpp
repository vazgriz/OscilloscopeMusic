#include "Line.h"
#include <fstream>

#define STAGING_BUFFER_SIZE (64 * 1024 * 1024)
#define VERTEX_BUFFER_SIZE (64 * 1024 * 1024)
#define INDEX_BUFFER_SIZE (64 * 1024 * 1024)

Line::Line(Renderer& renderer) {
    m_renderer = &renderer;
    m_device = &renderer.device();
    m_renderPass = &renderer.renderPass();

    createBuffers();
    createPipelineLayout();
    createPipeline();
}

void Line::render(float dt, vk::CommandBuffer& commandBuffer) {
    commandBuffer.bindPipeline(vk::PipelineBindPoint::Graphics, *m_pipeline);

    vk::Viewport viewport = {};
    viewport.width = static_cast<float>(m_renderer->width());
    viewport.height = static_cast<float>(m_renderer->height());
    viewport.maxDepth = 1;

    vk::Rect2D scissor = {};
    scissor.extent.width = m_renderer->width();
    scissor.extent.height = m_renderer->height();

    commandBuffer.setViewport(0, viewport);
    commandBuffer.setScissor(0, scissor);

    commandBuffer.draw(3, 1, 0, 0);
}

std::vector<char> Line::loadFile(const std::string& filename) {
    std::ifstream file(filename, std::fstream::ate | std::fstream::binary);
    size_t size = file.tellg();

    std::vector<char> data(size);

    file.seekg(0);
    file.read(data.data(), size);

    return data;
}

vk::ShaderModule Line::loadShader(const std::string& filename) {
    vk::ShaderModuleCreateInfo info = {};
    info.code = std::move(loadFile(filename));

    return vk::ShaderModule(*m_device, info);
}

vk::DeviceMemory Line::allocateMemory(vk::Buffer& buffer, vk::MemoryPropertyFlags required, vk::MemoryPropertyFlags preferred) {
    return m_renderer->allocateMemory(buffer.requirements(), required, preferred);
}

void Line::createBuffers() {
    {
        vk::BufferCreateInfo info = {};
        info.size = STAGING_BUFFER_SIZE;
        info.usage = vk::BufferUsageFlags::TransferSrc;

        m_stagingBuffer = std::make_unique<vk::Buffer>(m_renderer->device(), info);
        m_stagingBufferMemory = std::make_unique<vk::DeviceMemory>(allocateMemory(*m_stagingBuffer,
            vk::MemoryPropertyFlags::HostVisible | vk::MemoryPropertyFlags::HostCoherent,
            vk::MemoryPropertyFlags::DeviceLocal));
    }

    {
        vk::BufferCreateInfo info = {};
        info.size = VERTEX_BUFFER_SIZE;
        info.usage = vk::BufferUsageFlags::TransferDst | vk::BufferUsageFlags::VertexBuffer;

        m_vertexBuffer = std::make_unique<vk::Buffer>(m_renderer->device(), info);
        m_vertexBufferMemory = std::make_unique<vk::DeviceMemory>(allocateMemory(*m_stagingBuffer,
            vk::MemoryPropertyFlags::DeviceLocal,
            vk::MemoryPropertyFlags::None));
    }

    {
        vk::BufferCreateInfo info = {};
        info.size = INDEX_BUFFER_SIZE;
        info.usage = vk::BufferUsageFlags::TransferDst | vk::BufferUsageFlags::IndexBuffer;

        m_indexBuffer = std::make_unique<vk::Buffer>(m_renderer->device(), info);
        m_indexBufferMemory = std::make_unique<vk::DeviceMemory>(allocateMemory(*m_stagingBuffer,
        vk::MemoryPropertyFlags::DeviceLocal,
            vk::MemoryPropertyFlags::None));
    }

    {
        vk::BufferCreateInfo info = {};
        info.size = sizeof(UniformBuffer);
        info.usage = vk::BufferUsageFlags::TransferDst | vk::BufferUsageFlags::UniformBuffer;

        m_uniformBuffer = std::make_unique<vk::Buffer>(m_renderer->device(), info);
        m_uniformBufferMemory = std::make_unique<vk::DeviceMemory>(allocateMemory(*m_stagingBuffer,
            vk::MemoryPropertyFlags::HostVisible | vk::MemoryPropertyFlags::HostCoherent,
            vk::MemoryPropertyFlags::DeviceLocal));
    }
}

void Line::createPipelineLayout() {
    vk::PipelineLayoutCreateInfo info = {};

    m_pipelineLayout = std::make_unique<vk::PipelineLayout>(*m_device, info);
}

void Line::createPipeline() {
    vk::ShaderModule vertexShader = loadShader("shaders/line.vert.spv");
    vk::ShaderModule fragmentShader = loadShader("shaders/line.frag.spv");

    vk::PipelineShaderStageCreateInfo vertexStage = {};
    vertexStage.module = &vertexShader;
    vertexStage.name = "main";
    vertexStage.stage = vk::ShaderStageFlags::Vertex;

    vk::PipelineShaderStageCreateInfo fragmentStage = {};
    fragmentStage.module = &fragmentShader;
    fragmentStage.name = "main";
    fragmentStage.stage = vk::ShaderStageFlags::Fragment;

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};

    vk::PipelineInputAssemblyStateCreateInfo inputInfo = {};
    inputInfo.topology = vk::PrimitiveTopology::TriangleList;

    vk::PipelineViewportStateCreateInfo viewportInfo = {};
    viewportInfo.viewports = { {} };
    viewportInfo.scissors = { {} };

    vk::PipelineRasterizationStateCreateInfo rasterizationInfo = {};
    rasterizationInfo.polygonMode = vk::PolygonMode::Fill;
    rasterizationInfo.cullMode = vk::CullModeFlags::Back;
    rasterizationInfo.frontFace = vk::FrontFace::Clockwise;
    rasterizationInfo.lineWidth = 1.0f;

    vk::PipelineMultisampleStateCreateInfo multisampleInfo = {};
    multisampleInfo.rasterizationSamples = vk::SampleCountFlags::_1;

    vk::PipelineColorBlendAttachmentState colorBlendAttachmentInfo = {};
    colorBlendAttachmentInfo.colorWriteMask = vk::ColorComponentFlags::R
                                  | vk::ColorComponentFlags::G
                                  | vk::ColorComponentFlags::B
                                  | vk::ColorComponentFlags::A;

    vk::PipelineColorBlendStateCreateInfo colorBlendInfo = {};
    colorBlendInfo.attachments = { colorBlendAttachmentInfo };

    vk::PipelineDynamicStateCreateInfo dynamicInfo = {};
    dynamicInfo.dynamicStates = {
        vk::DynamicState::Viewport,
        vk::DynamicState::Scissor
    };

    vk::GraphicsPipelineCreateInfo info = {};
    info.stages = {
        vertexStage,
        fragmentStage
    };
    info.vertexInputState = &vertexInputInfo;
    info.inputAssemblyState = &inputInfo;
    info.viewportState = &viewportInfo;
    info.rasterizationState = &rasterizationInfo;
    info.multisampleState = &multisampleInfo;
    info.colorBlendState = &colorBlendInfo;
    info.dynamicState = &dynamicInfo;
    info.layout = m_pipelineLayout.get();
    info.renderPass = m_renderPass;

    m_pipeline = std::make_unique<vk::GraphicsPipeline>(*m_device, info);
}