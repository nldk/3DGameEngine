//
// Created by niel on 1/6/26.
//

#include "scene.h"
#include "renderer.h"
#include "engine.h"
#include <fstream>
#include <iostream>

Engine::Scene::Scene(const std::string& scenePath) : camera(nullptr) {
    // assign the incoming path to the member variable
    std::cout << "Scene created with path " << scenePath << "\n";
    if (scenePath.empty()) {
        return;
    }
    this->scenePath = scenePath;

    std::ifstream file(this->scenePath); // Open the file

    if (!file.is_open()) { // Check if the file opened successfully
        std::cerr << "Failed to open file: " << this->scenePath << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) { // Read line by line
        sceneClasses.push_back(line);
    }
    file.close();
    initCamera();
}

void Engine::Scene::enterScene() {
    for (auto& sceneClass: sceneClasses) {
        System* system = ClassRegistry::Instance().Create(sceneClass);
        if (!system) {
            std::cerr << "Failed to create system" + sceneClass + " or system doesn't exist.\n";
            return;
        }
        activeSystems.push_back(system);
    }
}
void Engine::Scene::updateScene(double delta) {
    // Use index-based loop so activeSystems can grow during iteration without invalidating references
    for (size_t i = 0; i < activeSystems.size(); ++i) {
        activeSystems[i]->RunUpdates(delta);
    }
}
void Engine::Scene::exitScene() {
    for (auto& sys: activeSystems) {
        delete sys;
    }
    activeSystems.clear();
}
void Engine::Scene::initCamera() {
    if (!camera) {
        camera = new Camera(Engine::Engine::Instance().defaultShaderProgram);
    }
}