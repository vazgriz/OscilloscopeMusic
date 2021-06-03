#include "Audio.h"
#include <stdexcept>
#include "App.h"

Audio::Audio(const char* filename, App& app) {
    m_app = &app;

    ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_f32, 2, SAMPLE_RATE);
    if (ma_decoder_init_file(filename, &decoderConfig, &m_decoder) != MA_SUCCESS) {
        throw std::runtime_error("Could not open file");
    }

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = m_decoder.outputFormat;
    deviceConfig.playback.channels = m_decoder.outputChannels;
    deviceConfig.sampleRate = m_decoder.outputSampleRate;
    deviceConfig.dataCallback = &Audio::audioCallback;
    deviceConfig.pUserData = this;

    if (ma_device_init(NULL, &deviceConfig, &m_device) != MA_SUCCESS) {
        ma_decoder_uninit(&m_decoder);
        throw std::runtime_error("Could not create audio device");
    }

    ma_device_start(&m_device);
}

Audio::~Audio() {
    ma_decoder_uninit(&m_decoder);
    ma_device_uninit(&m_device);
}

void Audio::audioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    Audio* audio = static_cast<Audio*>(pDevice->pUserData);
    if (audio == NULL) return;

    ma_decoder* pDecoder = &audio->m_decoder;

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);
    audio->m_app->addAudioSamples(frameCount, static_cast<AudioFrame*>(pOutput));
}