//
// Created by niel on 1/6/26.
//
#include "engine.h"

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
    running = false;
}

Engine::Engine::~Engine() {
    delete currentScene;
    currentScene = nullptr;
}

void Engine::Engine::runUpdateLoop(Window win) {
    float currentFrame = static_cast<float>(glfwGetTime());

    // Calculate delta time
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    while (running) {
        currentScene->updateScene(deltaTime);
        win.update();

        win.clearGLCollorBit();
    }
}
