//
// Created by niel on 1/6/26.
//
#include "engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Engine::Engine::Engine(const std::string& path) {
    running = false;
    currentScene = new Scene(path);
}

auto Engine::Engine::changeCurrentScene(std::string path) -> void {
    running = false;
    Scene* newScene = new Scene(path);
    delete currentScene;
    currentScene = newScene;
    running = true;
    currentScene->enterScene();
}

void Engine::Engine::start() {
    running = true;
    currentScene->enterScene();
}

void Engine::Engine::exit() {
    window->destroy();
    running = false;
}

Engine::Engine::~Engine() {
    delete currentScene;
    currentScene = nullptr;
}

void Engine::Engine::runUpdateLoop() {
    float currentFrame = static_cast<float>(glfwGetTime());

    // Calculate delta time
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    while (running) {
        currentScene->updateScene(deltaTime);
        window->update();

        window->clearGLCollorBit();
    }
}
