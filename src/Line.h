#pragma once
#include <VulkanWrapper/VulkanWrapper.h>
#include "Renderer.h"

class Line : public IRenderer {
public:
    Line(Renderer& renderer);
    Line(const Line& other) = delete;
    Line& operator = (const Line& other) = delete;
    Line(Line&& other) = default;
    Line& operator = (Line&& other) = default;

    void render(float dt, vk::CommandBuffer& commandBuffer) override;

private:
    struct UniformBuffer {
        float color[4];
    };

    Renderer* m_renderer;
    vk::Device* m_device;
    vk::RenderPass* m_renderPass;
    std::unique_ptr<vk::PipelineLayout> m_pipelineLayout;
    std::unique_ptr<vk::Pipeline> m_pipeline;

    std::unique_ptr<vk::Buffer> m_stagingBuffer;
    std::unique_ptr<vk::Buffer> m_vertexBuffer;
    std::unique_ptr<vk::Buffer> m_indexBuffer;
    std::unique_ptr<vk::Buffer> m_uniformBuffer;

    std::unique_ptr<vk::DeviceMemory> m_stagingBufferMemory;
    std::unique_ptr<vk::DeviceMemory> m_vertexBufferMemory;
    std::unique_ptr<vk::DeviceMemory> m_indexBufferMemory;
    std::unique_ptr<vk::DeviceMemory> m_uniformBufferMemory;

    std::vector<char> loadFile(const std::string& filename);
    vk::ShaderModule loadShader(const std::string& filename);

    vk::DeviceMemory allocateMemory(vk::Buffer& buffer, vk::MemoryPropertyFlags required, vk::MemoryPropertyFlags preferred);
    void createBuffers();

    void createPipelineLayout();
    void createPipeline();
};