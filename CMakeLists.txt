cmake_minimum_required(VERSION 4.1)
project(Niels3DGameEngine)

set(CMAKE_CXX_STANDARD 20)


add_executable(Niels3DGameEngine game/test.cpp
        game/test.cpp
        glad/glad.c
        glad/glad/glad.h
        glfw-3.4/include/GLFW/glfw3.h
        engine/window.cpp
        engine/window.h
        engine/utils.cpp
        engine/utils.h
        engine/Entry.cpp
        engine/classRegistarion.h
        engine/engine.h
        engine/scene.cpp
        engine/scene.h
        engine/engine.cpp
        engine/renderer.cpp
        engine/renderer.h
        engine/stb_image.h
)
target_include_directories(Niels3DGameEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/glad
)
find_package(glfw3 REQUIRED)

target_link_libraries(Niels3DGameEngine
        glfw
        GL
)