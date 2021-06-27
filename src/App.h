#pragma once
#include <memory>
#include <atomic>

#include "Audio.h"
#include "Renderer.h"
#include "Line.h"
#include "AudioBuffer.h"

struct GLFWwindow;

class App {
public:
    App(GLFWwindow* window, const char* filename);
    App(const App& other) = delete;
    App& operator = (const App& other) = delete;
    App(App&& other) = default;
    App& operator = (App&& other) = default;

    void waitIdle();

    bool isPaused() const { return m_paused; }
    bool isIconified() const { return m_iconified; }
    void update(float dt);

    void addAudioSamples(uint32_t frameCount, AudioFrame* frames);

private:
    std::unique_ptr<Audio> m_audio;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Line> m_line;
    ma_pcm_rb m_rawBuffer;
    AudioBuffer m_audioBuffer;
    size_t m_persistentFrame;
    std::atomic<bool> m_paused;
    bool m_iconified;

    uint32_t calculateFramesToRead(float dt);
    void readAudioFrames(float dt);

    static void handleWindowResize(GLFWwindow* window, int width, int height);
    static void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
    static void handleIconify(GLFWwindow* window, int iconified);
};