#pragma once
#include <vector>
#include "Audio.h"

//fixed size ring buffer
//oldest values are dropped when new data is appended
class AudioBuffer {
public:
    AudioBuffer(size_t capacity);
    AudioBuffer(const AudioBuffer& other) = delete;
    AudioBuffer& operator = (const AudioBuffer& other) = delete;
    AudioBuffer(AudioBuffer&& other) = default;
    AudioBuffer& operator = (AudioBuffer&& other) = default;

    void drop(size_t count);
    void push(AudioFrame frame);

    size_t capacity() const;
    size_t count() const;
    AudioFrame get(size_t index) const;

private:
    std::vector<AudioFrame> m_data;
    size_t m_capacity;
    size_t m_start;
    size_t m_count;

    size_t getRealIndex(size_t index) const;
};