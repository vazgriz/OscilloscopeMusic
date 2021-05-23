#pragma once
#include <miniaudio.h>

class Audio {
public:
    Audio(const char* filename);
    Audio(const Audio& other) = delete;
    Audio& operator = (const Audio& other) = delete;
    Audio(Audio&& other) = default;
    Audio& operator = (Audio&& other) = default;

    ~Audio();

private:
    ma_decoder m_decoder;
    ma_device m_device;

    static void audioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};