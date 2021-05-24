cmake_minimum_required (VERSION 3.8)

project ("OscilloscopeMusic")

set(GLFW_INCLUDE "" CACHE PATH "Path to GLFW include folder")
set(GLFW_LIB "" CACHE FILEPATH "Path to GLFW binaries (must be Vulkan static)")
set(VULKAN_INCLUDE "" CACHE PATH "Path to Vulkan include folder")
set(VULKAN_LIB "" CACHE FILEPATH "Path to Vulkan binary")
set(MINIAUDIO_PATH "" CACHE PATH "Path to miniaudio 0.10.35 root")
set(VULKAN_WRAPPER_INCLUDE "" CACHE PATH "Path to Vulkan Wrapper (VKW) include folder")
set(VULKAN_WRAPPER_LIB "" CACHE FILEPATH "Path to VKW binary")

add_executable("OscilloscopeMusic"
    "src/main.cpp"
    "src/miniaudio.cpp"
    "src/App.h"
    "src/App.cpp"
    "src/Audio.h"
    "src/Audio.cpp"
    "src/Renderer.h"
    "src/Renderer.cpp"
)

target_compile_features("OscilloscopeMusic" PRIVATE cxx_std_17)

target_include_directories("OscilloscopeMusic"
    PUBLIC ${GLFW_INCLUDE}
    PUBLIC ${VULKAN_INCLUDE}
    PUBLIC ${MINIAUDIO_PATH}
    PUBLIC ${VULKAN_WRAPPER_INCLUDE}
)

target_link_libraries("OscilloscopeMusic"
    ${GLFW_LIB}
    ${VULKAN_LIB}
    ${VULKAN_WRAPPER_LIB}
)