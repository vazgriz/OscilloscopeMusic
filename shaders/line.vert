#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UBO {
    mat4 proj;
    vec4 colorWidth;
} ubo;

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = ubo.proj * vec4(inPos + inNormal * ubo.colorWidth.w, 1.0);
    fragColor = ubo.colorWidth.xyz;
}

