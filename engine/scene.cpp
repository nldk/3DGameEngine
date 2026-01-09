//
// Created by niel on 1/6/26.
//

#include "scene.h"
#include <fstream>
#include <iostream>

Engine::Scene::Scene(const std::string& scenePath) {
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
    for (auto& sys: activeSystems) {
        sys->RunUpdates(delta);
    }
}
void Engine::Scene::exitScene() {
    for (auto& sys: activeSystems) {
        delete sys;
    }
    activeSystems.clear();
}