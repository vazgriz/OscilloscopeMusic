#include <iostream>
#include <GLFW/glfw3.h>

int main(int argc, const char** argv) {
    glfwInit();

    if (!glfwVulkanSupported) {
        std::cerr << "Vulkan is not supported" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Oscilloscope Music", nullptr, nullptr);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
