cmake_minimum_required (VERSION 3.8)

project ("OscilloscopeMusic")

set(GLFW_INCLUDE "" CACHE PATH "Path to GLFW include folder")
set(GLFW_LIB "" CACHE FILEPATH "Path to GLFW binaries (must be Vulkan static)")
set(VULKAN_INCLUDE "" CACHE PATH "Path to Vulkan include folder")
set(VULKAN_LIB "" CACHE FILEPATH "Path to Vulkan binary")
set(MINIAUDIO_PATH "" CACHE PATH "Path to miniaudio 0.10.35 root")

add_executable("OscilloscopeMusic"
    "src/main.cpp"
    "src/miniaudio.cpp"
    "src/App.h"
    "src/App.cpp"
    "src/Audio.h"
    "src/Audio.cpp"
)

target_include_directories("OscilloscopeMusic"
    PUBLIC ${GLFW_INCLUDE}
    PUBLIC ${VULKAN_INCLUDE}
    PUBLIC ${MINIAUDIO_PATH}
)

target_link_libraries("OscilloscopeMusic"
    ${GLFW_LIB}
    ${VULKAN_LIB}
)