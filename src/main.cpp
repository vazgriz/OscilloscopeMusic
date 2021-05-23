#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "App.h"

int main(int argc, const char** argv) {
    try {
        glfwInit();

        if (argc < 2) {
            std::cerr << "Must specify a file name" << std::endl;
            return 1;
        }

        {
            //check if file exists
            std::ifstream file(argv[1]);
            if (!file.good()) {
                std::cerr << "Could not open file" << std::endl;
                return 1;
            }
        }

        if (!glfwVulkanSupported) {
            std::cerr << "Vulkan is not supported" << std::endl;
            return 1;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(800, 600, "Oscilloscope Music", nullptr, nullptr);

        App app(argv[1]);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }
    catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
