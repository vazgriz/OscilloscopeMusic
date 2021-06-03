#include "App.h"
#include <GLFW/glfw3.h>

App::App(GLFWwindow* window, const char* filename) {
    auto result = ma_pcm_rb_init(ma_format_f32, 2, SAMPLE_RATE / 60 * 2, nullptr, nullptr, &m_ringBuffer);

    m_audio = std::make_unique<Audio>(filename, *this);
    m_renderer = std::make_unique<Renderer>(window);
    m_line = std::make_unique<Line>(m_renderer->device());

    m_renderer->addRenderer(*m_line);
}

void App::update(float dt) {
    readAudioFrames(dt);
    m_renderer->render(dt);
}

void App::addAudioSamples(uint32_t frameCount, AudioFrame* frames) {
    ma_uint32 writeRemaining = frameCount;

    while (writeRemaining > 0) {
        void* writePtr;
        ma_uint32 framesToWrite = writeRemaining;
        ma_pcm_rb_acquire_write(&m_ringBuffer, &framesToWrite, &writePtr);
        memcpy(writePtr, &frames[frameCount - writeRemaining], sizeof(AudioFrame) * framesToWrite);
        ma_pcm_rb_commit_write(&m_ringBuffer, framesToWrite, writePtr);
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
        ma_pcm_rb_acquire_read(&m_ringBuffer, &framesToRead, &readPtr);
        memcpy(&buffer, readPtr, sizeof(AudioFrame) * framesToRead);
        ma_pcm_rb_commit_read(&m_ringBuffer, framesToRead, readPtr);
        if (framesToRead == 0) break;
        readRemaining -= framesToRead;
    }
}