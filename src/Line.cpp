#include "Line.h"

Line::Line(vk::Device& device) {
    m_device = &device;

    createPipeline();
}

void Line::render(float dt) {

}

void Line::createPipeline() {

}