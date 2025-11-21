cmake_minimum_required(VERSION 3.3)
project(FractalApp)

set(CMAKE_CXX_STANDARD 17)

# Copy shaders to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(OpenGL REQUIRED)

add_executable(fractal_app
    src/main.cpp
    src/fractal_utils.cpp
)

target_include_directories(fractal_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(fractal_app
    glfw
    GL
    GLEW
)

