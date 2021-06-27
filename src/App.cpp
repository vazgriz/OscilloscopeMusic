#include "App.h"
#include <GLFW/glfw3.h>

#define SAMPLES_PER_FRAME (SAMPLE_RATE / 60)
#define PERSISTENCE 4

App::App(GLFWwindow* window, const char* filename) : m_audioBuffer(SAMPLES_PER_FRAME * PERSISTENCE) {
    m_paused = false;
    m_iconified = false;
    m_persistentFrame = 0;

    glfwSetWindowUserPointer(window, this);

    auto result = ma_pcm_rb_init(ma_format_f32, 2, SAMPLES_PER_FRAME * 2, nullptr, nullptr, &m_rawBuffer);

    m_audio = std::make_unique<Audio>(filename, *this);
    m_renderer = std::make_unique<Renderer>(window);
    m_line = std::make_unique<Line>(m_audioBuffer.capacity(), PERSISTENCE, *m_renderer);

    m_renderer->addRenderer(*m_line);

    glfwSetWindowSizeCallback(window, &App::handleWindowResize);
    glfwSetMouseButtonCallback(window, &App::handleMouseButton);
    glfwSetWindowIconifyCallback(window, &App::handleIconify);
}

void App::waitIdle() {
    m_renderer->waitIdle();
}

void App::update(float dt) {
    if (!isPaused()) {
        readAudioFrames(dt);
    }

    m_renderer->render(dt);
}

void App::addAudioSamples(uint32_t frameCount, AudioFrame* frames) {
    ma_uint32 writeRemaining = frameCount;

    while (writeRemaining > 0) {
        void* writePtr;
        ma_uint32 framesToWrite = writeRemaining;
        ma_pcm_rb_acquire_write(&m_rawBuffer, &framesToWrite, &writePtr);
        memcpy(writePtr, &frames[frameCount - writeRemaining], sizeof(AudioFrame) * framesToWrite);
        ma_pcm_rb_commit_write(&m_rawBuffer, framesToWrite, writePtr);
        if (framesToWrite == 0) break;
        writeRemaining -= framesToWrite;
    }
}

uint32_t App::calculateFramesToRead(float dt) {
    return static_cast<uint32_t>(ceil(SAMPLE_RATE * dt));
}

void App::readAudioFrames(float dt) {
    uint32_t frameCount = calculateFramesToRead(dt);
    ma_uint32 readRemaining = frameCount;

    AudioFrame buffer[512];

    while (readRemaining > 0) {
        void* readPtr;
        ma_uint32 framesToRead = std::min<uint32_t>(readRemaining, 512);
        ma_pcm_rb_acquire_read(&m_rawBuffer, &framesToRead, &readPtr);
        memcpy(&buffer, readPtr, sizeof(AudioFrame) * framesToRead);
        ma_pcm_rb_commit_read(&m_rawBuffer, framesToRead, readPtr);
        if (framesToRead == 0) break;
        readRemaining -= framesToRead;

        if (m_audioBuffer.count() + framesToRead > m_audioBuffer.capacity()) {
            m_audioBuffer.drop((m_audioBuffer.count() + framesToRead) - m_audioBuffer.capacity());
        }

        for (uint32_t i = 0; i < framesToRead; i++) {
            m_audioBuffer.push(buffer[i]);
        }
    }

    for (size_t i = 0; i < m_audioBuffer.count(); i++) {
        AudioFrame frame = m_audioBuffer.get(i);
        m_line->addPoint(frame.sample[0], frame.sample[1]);
    }
}

void App::handleWindowResize(GLFWwindow* window, int width, int height) {
    App& app = *static_cast<App*>(glfwGetWindowUserPointer(window));

    if (width != 0 && height != 0) {
        app.m_renderer->resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }
}

void App::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    App& app = *static_cast<App*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        app.m_paused = !app.m_paused;
    }
}

void App::handleIconify(GLFWwindow* window, int iconified) {
    App& app = *static_cast<App*>(glfwGetWindowUserPointer(window));
    app.m_paused = iconified == 1;
    app.m_iconified = iconified == 1;
}