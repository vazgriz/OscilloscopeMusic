#pragma once
#include <memory>
#include <VulkanWrapper/VulkanWrapper.h>
#include <optional>

struct GLFWwindow;

class Renderer {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;

        bool isComplete();
    };

public:
    Renderer(GLFWwindow* window);
    Renderer(const Renderer& other) = delete;
    Renderer& operator = (const Renderer& other) = delete;
    Renderer(Renderer&& other) = default;
    Renderer& operator = (Renderer&& other) = default;

private:
    GLFWwindow* m_window;
    uint32_t m_width;
    uint32_t m_height;

    std::unique_ptr<vk::Instance> m_instance;
    std::unique_ptr<vk::Surface> m_surface;
    const vk::PhysicalDevice* m_physicalDevice;
    std::unique_ptr<vk::Device> m_device;
    std::unique_ptr<vk::Swapchain> m_swapchain;
    std::vector<vk::ImageView> m_imageViews;

    std::vector<std::string> getRequiredExtensions(GLFWwindow* window);
    bool validationLayersSupported();
    QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device);
    bool swapchainSupported(const vk::PhysicalDevice& physicalDevice);
    bool isDeviceSuitable(const vk::PhysicalDevice& physicalDevice);

    vk::SurfaceFormat chooseFormat();
    vk::PresentMode choosePresentMode();
    vk::Extent2D chooseExtent(vk::SurfaceCapabilities& capabilities);

    void createInstance();
    void createSurface();
    void createDevice();
    void createSwapchain();
    void createImageViews();

    void recreateSwapchain();
};