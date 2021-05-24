#pragma once
#include <memory>

#include "Audio.h"
#include "Renderer.h"

struct GLFWwindow;

class App {
public:
    App(GLFWwindow* window, const char* filename);
    App(const App& other) = delete;
    App& operator = (const App& other) = delete;
    App(App&& other) = default;
    App& operator = (App&& other) = default;

private:
    std::unique_ptr<Audio> m_audio;
    std::unique_ptr<Renderer> m_renderer;
};