#pragma once
#include <miniaudio.h>

#define SAMPLE_RATE 192000

class App;

struct AudioFrame {
    float sample[2];
};

class Audio {
public:
    Audio(const char* filename, App& app);
    Audio(const Audio& other) = delete;
    Audio& operator = (const Audio& other) = delete;
    Audio(Audio&& other) = default;
    Audio& operator = (Audio&& other) = default;

    ~Audio();

private:
    App* m_app;
    ma_decoder m_decoder;
    ma_device m_device;

    static void audioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};