#include "App.h"

App::App(const char* filename) {
    m_audio = std::make_unique<Audio>(filename);
}