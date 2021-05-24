#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <unordered_set>

std::vector<std::string> layerNames = {
#ifndef NDEBUG
    "VK_LAYER_KHRONOS_validation"
#endif
};

std::vector<std::string> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

bool Renderer::QueueFamilyIndices::isComplete() {
    return graphics.has_value() && present.has_value();
}

Renderer::Renderer(GLFWwindow* window) {
    m_window = window;
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);

    createInstance();
    createSurface();
    createDevice();
    recreateSwapchain();
}

std::vector<std::string> Renderer::getRequiredExtensions(GLFWwindow* window) {
    uint32_t extensionCount = 0;
    const char** requiredExtensions;
    requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    std::vector<std::string> extensions;

    for (uint32_t i = 0; i < extensionCount; i++) {
        extensions.push_back(requiredExtensions[i]);
    }

    return extensions;
}

bool Renderer::validationLayersSupported() {
    for (auto& layer : vk::Instance::availableLayers()) {
        if (layer.layerName == "VK_LAYER_KHRONOS_validation") {
            return true;
        }
    }

    return false;
}

void Renderer::createInstance() {
    vk::ApplicationInfo appInfo = {};
    appInfo.apiVersion = VK_MAKE_VERSION(1, 2, 0);
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.applicationName = "Oscilloscope Music";

    vk::InstanceCreateInfo info = {};
    info.applicationInfo = &appInfo;
    info.enabledExtensionNames = getRequiredExtensions(m_window);

    if (validationLayersSupported()) {
        info.enabledLayerNames = layerNames;
    }

    m_instance = std::make_unique<vk::Instance>(info);
}

void Renderer::createSurface() {
    VkSurfaceKHR surface;
    glfwCreateWindowSurface(m_instance->handle(), m_window, nullptr, &surface);

    m_surface = std::make_unique<vk::Surface>(*m_instance, surface);
}

Renderer::QueueFamilyIndices Renderer::findQueueFamilies(const vk::PhysicalDevice& device) {
    QueueFamilyIndices indices;

    for (uint32_t i = 0; i < device.queueFamilies().size(); i++) {
        const auto& queueFamily = device.queueFamilies()[i];

        if ((queueFamily.queueFlags & vk::QueueFlags::Graphics) != vk::QueueFlags::None) {
            indices.graphics = i;
        }

        if (m_surface->supported(device, i)) {
            indices.present = i;
        }

        if (indices.isComplete()) {
            break;
        }
    }

    return indices;
}

bool Renderer::swapchainSupported(const vk::PhysicalDevice& physicalDevice) {
    std::unordered_set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : physicalDevice.availableExtensions()) {
        requiredExtensions.erase(extension.extensionName);
    }

    if (requiredExtensions.size() > 0) return false;
    if (m_surface->getFormats(physicalDevice).size() == 0) return false;
    if (m_surface->getPresentModes(physicalDevice).size() == 0) return false;

    return true;
}

bool Renderer::isDeviceSuitable(const vk::PhysicalDevice& physicalDevice) {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    return indices.isComplete() && swapchainSupported(physicalDevice);
}

void Renderer::createDevice() {
    m_physicalDevice = nullptr;

    for (const auto& device : m_instance->physicalDevices()) {
        if (isDeviceSuitable(device)) {
            m_physicalDevice = &device;
            break;
        }
    }

    if (m_physicalDevice == nullptr) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }

    QueueFamilyIndices indices = findQueueFamilies(*m_physicalDevice);
    std::unordered_set<uint32_t> uniqueIndices = { indices.graphics.value(), indices.present.value() };
    std::vector<vk::DeviceQueueCreateInfo> queueInfos;

    for (auto index : uniqueIndices) {
        vk::DeviceQueueCreateInfo queueInfo = {};
        queueInfo.queueFamilyIndex = index;
        queueInfo.queueCount = 1;
        queueInfo.queuePriorities = { 1.0f };

        queueInfos.emplace_back(std::move(queueInfo));
    }

    vk::DeviceCreateInfo info = {};
    info.queueCreateInfos = queueInfos;
    info.enabledExtensionNames = deviceExtensions;

    m_device = std::make_unique<vk::Device>(*m_physicalDevice, info);
}

vk::SurfaceFormat Renderer::chooseFormat() {
    auto& formats = m_surface->getFormats(*m_physicalDevice);

    for (const auto& surfaceFormat : formats) {
        if (surfaceFormat.format == vk::Format::R8G8B8A8_Srgb && surfaceFormat.colorSpace == vk::ColorSpace::SrgbNonlinear) {
            return surfaceFormat;
        }
    }

    return formats[0];
}

vk::PresentMode Renderer::choosePresentMode() {
    return vk::PresentMode::Fifo;
}

vk::Extent2D Renderer::chooseExtent(vk::SurfaceCapabilities& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = { m_width, m_height };

        actualExtent.width = std::clamp(m_width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(m_height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void Renderer::createSwapchain() {
    auto capabilities = m_surface->getCapabilities(*m_physicalDevice);

    vk::SurfaceFormat surfaceFormat = chooseFormat();
    vk::PresentMode presentMode = choosePresentMode();
    vk::Extent2D extent = chooseExtent(capabilities);

    vk::SwapchainCreateInfo info = {};
    info.surface = m_surface.get();
    info.imageFormat = surfaceFormat.format;
    info.imageColorSpace = surfaceFormat.colorSpace;
    info.presentMode = presentMode;
    info.imageExtent = extent;
    info.minImageCount = std::clamp<uint32_t>(2, capabilities.minImageCount, capabilities.maxImageCount);
    info.imageArrayLayers = 1;
    info.imageUsage = vk::ImageUsageFlags::ColorAttachment;

    QueueFamilyIndices indices = findQueueFamilies(*m_physicalDevice);

    if (indices.graphics != indices.present) {
        info.imageSharingMode = vk::SharingMode::Concurrent;
        info.queueFamilyIndices = { indices.graphics.value(), indices.present.value() };
    } else {
        info.imageSharingMode = vk::SharingMode::Exclusive;
    }

    info.preTransform = capabilities.currentTransform;
    info.compositeAlpha = vk::CompositeAlphaFlags::Opaque;
    info.oldSwapchain = m_swapchain.get();

    m_swapchain = std::make_unique<vk::Swapchain>(*m_device, info);
}

void Renderer::createImageViews() {
    m_imageViews.clear();

    for (auto& image : m_swapchain->images()) {
        vk::ImageViewCreateInfo info = {};
        info.image = &image;
        info.format = image.format();
        info.viewType = vk::ImageViewType::_2D;
        info.subresourceRange.aspectMask = vk::ImageAspectFlags::Color;
        info.subresourceRange.layerCount = 1;
        info.subresourceRange.levelCount = 1;

        m_imageViews.emplace_back(*m_device, info);
    }
}

void Renderer::recreateSwapchain() {
    createSwapchain();
    createImageViews();
}