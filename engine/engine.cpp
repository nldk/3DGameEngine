//
// Created by niel on 1/6/26.
//
#include "engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "renderer.h"

Engine::Engine::Engine(const std::string& path) {
    running = false;
    currentScene = new Scene(path);
    defaultShaderProgram = new ShaderProgram();
    Shader vshader = Shader("engine/shaders/shader.v",GL_VERTEX_SHADER,"vertex");
    Shader fshader = Shader("engine/shaders/shader.f",GL_FRAGMENT_SHADER,"fragment");
    defaultShaderProgram->addShader(&vshader);
    defaultShaderProgram->addShader(&fshader);
    defaultShaderProgram->compile();
    SpriteRenderer = new Renderer(defaultShaderProgram);
}

auto Engine::Engine::changeCurrentScene(const std::string& path) -> void {
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
    while (running) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (window) {
            window->clearGLCollorBit();
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        if (currentScene) {
            currentScene->updateScene(deltaTime);
        }
        if (window) {
            window->update();
        }
    }
}
