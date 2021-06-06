#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragLineCenter;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UBO {
    mat4 proj;
    vec4 colorWidth;
    vec2 screenSize;
} ubo;

void main() {
    float dist = length(fragLineCenter - gl_FragCoord.xy);
    float alpha = clamp(ubo.colorWidth.w - dist, 0, 1);
    outColor = vec4(ubo.colorWidth.xyz, alpha);
}