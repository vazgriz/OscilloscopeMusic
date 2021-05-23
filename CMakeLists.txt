cmake_minimum_required (VERSION 3.8)

project ("OscilloscopeMusic")

set(GLFW_INCLUDE "" CACHE PATH "Path to GLFW include folder")
set(GLFW_LIB "" CACHE FILEPATH "Path to GLFW binaries (must be Vulkan static)")
set(VULKAN_INCLUDE "" CACHE PATH "Path to Vulkan include folder")
set(VULKAN_LIB "" CACHE FILEPATH "Path to Vulkan binary")

add_executable("OscilloscopeMusic"
    "src/main.cpp"
)

target_include_directories("OscilloscopeMusic"
    PUBLIC ${GLFW_INCLUDE}
    PUBLIC ${VULKAN_INCLUDE}
)

target_link_libraries("OscilloscopeMusic"
    ${GLFW_LIB}
    ${VULKAN_LIB}
)