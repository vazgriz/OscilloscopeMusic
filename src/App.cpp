#include "App.h"
#include <GLFW/glfw3.h>

App::App(GLFWwindow* window, const char* filename) {
    m_audio = std::make_unique<Audio>(filename);
    m_renderer = std::make_unique<Renderer>(window);
}