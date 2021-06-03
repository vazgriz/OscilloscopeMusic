#pragma once
#include <memory>
#include <VulkanWrapper/VulkanWrapper.h>
#include <optional>

struct GLFWwindow;

class IRenderer {
public:
    virtual void render(float dt, vk::CommandBuffer& commandBuffer) = 0;
};

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

    void waitIdle();

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    vk::Device& device() const { return *m_device; }
    vk::RenderPass& renderPass() const { return *m_renderPass; }

    void render(float dt);

    void addRenderer(IRenderer& renderer);

private:
    GLFWwindow* m_window;
    uint32_t m_width;
    uint32_t m_height;

    uint32_t m_graphicsQueueIndex;
    uint32_t m_presentQueueIndex;
    const vk::Queue* m_graphicsQueue;
    const vk::Queue* m_presentQueue;

    std::unique_ptr<vk::Instance> m_instance;
    std::unique_ptr<vk::Surface> m_surface;
    const vk::PhysicalDevice* m_physicalDevice;
    std::unique_ptr<vk::Device> m_device;
    std::unique_ptr<vk::Swapchain> m_swapchain;
    std::vector<vk::ImageView> m_imageViews;
    std::unique_ptr<vk::RenderPass> m_renderPass;
    std::vector<vk::Framebuffer> m_framebuffers;

    std::unique_ptr<vk::CommandPool> m_commandPool;
    std::vector<vk::CommandBuffer> m_commandBuffers;

    std::unique_ptr<vk::Semaphore> m_acquireSemaphore;
    std::unique_ptr<vk::Semaphore> m_renderSemaphore;
    std::vector<vk::Fence> m_fences;

    std::vector<std::string> getRequiredExtensions(GLFWwindow* window);
    bool validationLayersSupported();
    QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device);
    bool swapchainSupported(const vk::PhysicalDevice& physicalDevice);
    bool isDeviceSuitable(const vk::PhysicalDevice& physicalDevice);

    vk::SurfaceFormat chooseFormat();
    vk::PresentMode choosePresentMode();
    vk::Extent2D chooseExtent(vk::SurfaceCapabilities& capabilities);

    std::vector<IRenderer*> m_renderers;

    void createInstance();
    void createSurface();
    void createDevice();
    void createSwapchain();
    void createImageViews();
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSemaphores();
    void createFences();

    void recreateSwapchain();

    uint32_t acquireImage();
    vk::CommandBuffer& recordCommandBuffer(float dt, uint32_t index, vk::Fence& fence);
    void submitCommandBuffer(vk::CommandBuffer& commandBuffer, const vk::Fence& fence);
    void presentImage(uint32_t index);
};