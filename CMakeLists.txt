cmake_minimum_required(VERSION 3.16)
project(Niels3DGameEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
# Dependencies
find_package(OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS audio system REQUIRED)
find_package(glfw3 QUIET)
if (NOT glfw3_FOUND)
    message(STATUS "glfw3 not found; building vendored glfw-3.4")
    add_subdirectory(glfw-3.4)
endif ()

add_executable(Niels3DGameEngine
        game/test.cpp
        glad/glad.c
        glad/glad.h
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
        engine/phisycs.cpp
        engine/phisycs.h
        engine/input.cpp
        engine/input.h
        engine/soundEngine.cpp
        engine/soundEngine.h
        engine/Engine.h
)

target_include_directories(Niels3DGameEngine PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/glad
)

target_link_libraries(Niels3DGameEngine
        glfw
        OpenGL::GL
        sfml-system
        sfml-audio
)