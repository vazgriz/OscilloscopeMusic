#include "AudioBuffer.h"

AudioBuffer::AudioBuffer(size_t capacity) {
    m_data.resize(capacity);
    m_capacity = capacity;
    m_start = 0;
    m_count = 0;
}

size_t AudioBuffer::getRealIndex(size_t index) const {
    return (m_start + index) % m_capacity;
}

void AudioBuffer::drop(size_t count) {
    count = std::min<size_t>(m_count, count);
    m_start = getRealIndex(count);
    m_count -= count;
}

void AudioBuffer::push(AudioFrame frame) {
    if (m_count == m_capacity) {
        drop(1);
    }

    size_t index = getRealIndex(m_count);
    m_data[index] = frame;
    m_count++;
}

size_t AudioBuffer::capacity() const {
    return m_capacity;
}

size_t AudioBuffer::count() const {
    return m_count;
}

AudioFrame AudioBuffer::get(size_t index) const {
    return m_data[getRealIndex(index)];
}