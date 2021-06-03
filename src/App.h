#pragma once
#include <memory>

#include "Audio.h"
#include "Renderer.h"
#include "Line.h"

struct GLFWwindow;

class App {
public:
    App(GLFWwindow* window, const char* filename);
    App(const App& other) = delete;
    App& operator = (const App& other) = delete;
    App(App&& other) = default;
    App& operator = (App&& other) = default;

    void update(float dt);

    void addAudioSamples(uint32_t frameCount, AudioFrame* frames);

private:
    std::unique_ptr<Audio> m_audio;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Line> m_line;
    ma_pcm_rb m_ringBuffer;

    uint32_t calculateFramesToRead(float dt);
    void readAudioFrames(float dt);
};