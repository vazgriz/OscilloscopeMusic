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
    Renderer* m_renderer;
    vk::Device* m_device;
    vk::RenderPass* m_renderPass;
    std::unique_ptr<vk::PipelineLayout> m_pipelineLayout;
    std::unique_ptr<vk::Pipeline> m_pipeline;

    std::vector<char> loadFile(const std::string& filename);
    vk::ShaderModule loadShader(const std::string& filename);

    void createPipelineLayout();
    void createPipeline();
};