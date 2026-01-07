//
// Created by niel on 1/5/26.
//

#include "window.h"

#include <iostream>
#include <ostream>

#include "engine.h"

Engine::Window::Window(int width, int height, const char *title, int gl_version_major, int gl_Version_minor) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_Version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void Engine::Window::update() {
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)) {
        Engine::Engine::Instance().exit();
    }
}

void Engine::Window::destroy() {
    glfwDestroyWindow(window);
}

void Engine::Window::setGLViewport(int x, int y, int width, int height) {
    viewportX = x;
    viewportY = y;
    glViewport(x,y,width,height);
}
void Engine::Window::resize(int x,int y, int width,int height) {
    this->setGLViewport(x,y,width,height);
}

bool Engine::Window::isKeyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Engine::Window::isKeyReleased(int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

void Engine::Window::clearGLCollorBit() {
    glClearColor(background.x, background.y, background.z, background.w);
    glClear(GL_COLOR_BUFFER_BIT);
}
int WindowStartupConfig::width,WindowStartupConfig::height;

std::string WindowStartupConfig::title;
int WindowStartupConfig::gl_version_major,WindowStartupConfig::gl_version_minor;
WindowStartupConfig::WindowStartupConfig(int width, int height, std::string title) {
    width = width;
    height = height;
    title = title;
}
