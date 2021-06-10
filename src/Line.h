#pragma once
#include <VulkanWrapper/VulkanWrapper.h>
#include "Renderer.h"
#include <glm/glm.hpp>

struct UniformBuffer {
    glm::mat4 projection;
    glm::vec4 colorWidth;
    glm::vec2 screenSize;
};

struct Vertex {
    glm::vec4 positionPower;
    glm::vec3 normal;
};

class Line : public IRenderer {
public:
    Line(Renderer& renderer);
    Line(const Line& other) = delete;
    Line& operator = (const Line& other) = delete;
    Line(Line&& other) = default;
    Line& operator = (Line&& other) = default;

    void addPoint(float x, float y);

    void render(float dt, vk::CommandBuffer& commandBuffer) override;

private:
    struct Transfer {
        vk::Buffer* buffer;
        vk::BufferCopy copy;
        vk::BufferMemoryBarrier barrier;
        vk::PipelineStageFlags stage;
    };

    std::vector<glm::vec3> m_points;
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    Renderer* m_renderer;
    vk::Device* m_device;
    vk::RenderPass* m_renderPass;
    std::unique_ptr<vk::DescriptorPool> m_descriptorPool;
    std::unique_ptr<vk::DescriptorSetLayout> m_descriptorSetLayout;
    std::unique_ptr<vk::DescriptorSet> m_descriptorSet;
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

    char* m_stagingPtr;
    size_t m_stagingOffset = 0;
    std::vector<Transfer> m_transfers;

    UniformBuffer* m_uniformBufferPtr;

    vk::DeviceMemory allocateMemory(vk::Buffer& buffer, vk::MemoryPropertyFlags required, vk::MemoryPropertyFlags preferred);
    void createBuffers();

    void transferData(size_t size, void* data, vk::Buffer& destinationBuffer, vk::AccessFlags destinationAccess, vk::PipelineStageFlags stage);
    void handleTransfers(vk::CommandBuffer& commandBuffer);

    void updateUniformBuffer();
    void createMesh();

    void createDescriptorPool();
    void createDescriptorSetLayout();
    void createDescriptor();
    void writeDescriptor();
    void createPipelineLayout();
    void createPipeline();
};