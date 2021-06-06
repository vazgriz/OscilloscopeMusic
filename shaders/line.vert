#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec2 fragLineCenter;

layout(binding = 0) uniform UBO {
    mat4 proj;
    vec4 colorWidth;
    vec2 screenSize;
} ubo;

void main() {
    gl_Position = ubo.proj * vec4(inPos + inNormal * ubo.colorWidth.w, 1.0);
    vec2 clip = (ubo.proj * vec4(inPos.xy, 0.0, 1.0)).xy;
    fragLineCenter = ((clip + vec2(1.0, 1.0)) / 2.0) * ubo.screenSize;
}