#pragma once
#include <memory>

#include "Audio.h"

class App {
public:
    App(const char* filename);
    App(const App& other) = delete;
    App& operator = (const App& other) = delete;
    App(App&& other) = default;
    App& operator = (App&& other) = default;

private:
    std::unique_ptr<Audio> m_audio;
};