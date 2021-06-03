#pragma once
#include <VulkanWrapper/VulkanWrapper.h>
#include "Renderer.h"

class Line : public IRenderer {
public:
    Line(vk::Device& device);
    Line(const Line& other) = delete;
    Line& operator = (const Line& other) = delete;
    Line(Line&& other) = default;
    Line& operator = (Line&& other) = default;

    void render(float dt);

private:
    vk::Device* m_device;

    void createPipeline();
};