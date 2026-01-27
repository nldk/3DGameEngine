//
// Created by niel on 1/6/26.
//

#include "scene.h"
#include "renderer.h"
#include "engine.h"
#include "phisycs.h"
#include <fstream>
#include <iostream>
#include <typeinfo>
#ifndef NDEBUG
#include <cxxabi.h>
#include <memory>
#include <cstdlib>
namespace {
    std::string demangle(const char* mangled) {
        int status = 0;
        std::unique_ptr<char, void(*)(void*)> result(
            abi::__cxa_demangle(mangled, nullptr, nullptr, &status),
            std::free);
        if (status == 0 && result) {
            return result.get();
        }
        return mangled ? mangled : "<null>";
    }
}
#endif

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
        addActiveSystem(system, sceneClass);
    }
}
void Engine::Scene::updateScene(double delta) {
    // Use index-based loop so activeSystems can grow during iteration without invalidating references
    for (size_t i = 0; i < activeSystems.size(); ++i) {
        activeSystems[i]->RunUpdates(delta);
    }

#ifndef NDEBUG
    if (Engine::Engine::Instance().getDebugDrawHitboxes()) {
        auto* debugRenderer = Engine::Engine::Instance().getDebugRenderer2D();
        Camera* cam = getCamera();
        if (debugRenderer && cam) {
            const auto physicsSystems = getSystemsOfType<PhysicsObject2D>();
            for (auto* phys : physicsSystems) {
                debugRenderer->drawHitboxes(phys->getWorldHitboxes(), *cam, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
            const auto tileMaps = getSystemsOfType<TileMap2D>();
            for (auto* tilemap : tileMaps) {
                for (auto* tile : tilemap->tiles) {
                    debugRenderer->drawHitboxes(tile->getWorldHitboxes(), *cam, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
                }
            }
        }
    }
#endif
}
void Engine::Scene::exitScene() {
    for (auto& sys: activeSystems) {
        delete sys;
    }
    activeSystems.clear();
}
void Engine::Scene::addActiveSystem(System* system, const std::string& name) {
    activeSystems.push_back(system);
#ifndef NDEBUG
    const char* typeName = system ? typeid(*system).name() : "<null>";
    const std::string displayName = !name.empty() ? name : demangle(typeName);
    std::cout << "[Scene] Added system '" << displayName << "' @" << system << std::endl;
#endif
}
void Engine::Scene::initCamera() {
    if (!camera) {
        camera = new Camera(Engine::Engine::Instance().defaultShaderProgram);
    }
}