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
    debugHitboxRenderer = new DebugRenderer2D(defaultShaderProgram);
#ifndef NDEBUG
    debugDrawHitboxes = true;
#endif

    // Enable alpha blending for 2D sprites
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

auto Engine::Engine::changeCurrentScene(const std::string& path) -> void {
    // If we are not running yet (initial load), swap immediately.
    if (!running) {
        Scene* newScene = new Scene(path);
        delete currentScene;
        currentScene = newScene;
        return;
    }
    // Defer hot scene switches until the end of the frame to avoid
    // destroying systems while they are mid-update.
    pendingScenePath = path;
    sceneChangeRequested = true;
}

void Engine::Engine::start() {
    running = true;
    glfwSwapInterval(WindowStartupConfig::vSync);
    if (currentScene) {
        currentScene->enterScene();
    }
}

void Engine::Engine::exit() {
    window->destroy();
    running = false;
}

Engine::Engine::~Engine() {
    delete debugHitboxRenderer;
    debugHitboxRenderer = nullptr;
    delete SpriteRenderer;
    SpriteRenderer = nullptr;
    delete defaultShaderProgram;
    defaultShaderProgram = nullptr;
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
        if (sceneChangeRequested) {
            Scene* newScene = new Scene(pendingScenePath);
            delete currentScene;
            currentScene = newScene;
            pendingScenePath.clear();
            sceneChangeRequested = false;
            if (currentScene) {
                currentScene->enterScene();
            }
            lastFrame = static_cast<float>(glfwGetTime());
        }
    }
    for (auto& sys: currentScene->getActiveSystems()) {
        std::cout << "active systhem"<< sys << std::endl;
    }
}
