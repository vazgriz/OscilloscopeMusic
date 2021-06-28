#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inPosAlpha;
layout(location = 1) in vec4 inNormalWidth;

layout(location = 0) out vec2 fragLineCenter;
layout(location = 1) out vec2 fragWidthAlpha;

layout(binding = 0) uniform UBO {
    mat4 proj;
    vec4 colorWidth;
    vec2 screenSize;
} ubo;

void main() {
    //expand vertices along normals
    gl_Position = ubo.proj * vec4(inPosAlpha.xyz + inNormalWidth.xyz * ubo.colorWidth.w * inNormalWidth.w, 1.0);

    //calculate where the center of each line segment is (project without expanding)
    vec2 clip = (ubo.proj * vec4(inPosAlpha.xy, 0.0, 1.0)).xy;
    fragLineCenter = ((clip + vec2(1.0, 1.0)) / 2.0) * ubo.screenSize;

    //pass width and alpha
    fragWidthAlpha = vec2(inNormalWidth.w, inPosAlpha.w * inNormalWidth.w);
}