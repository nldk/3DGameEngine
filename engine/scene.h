//
// Created by niel on 1/6/26.
//

#ifndef NIELS3DGAMEENGINE_SCENE_H
#define NIELS3DGAMEENGINE_SCENE_H
#include <string>
#include <vector>
#include <typeinfo>
#include <memory>
#include <cstdlib>
#include <cxxabi.h>

#include "classRegistarion.h"

namespace Engine {
    class Camera;

    class Scene {
        public:
        Scene(const std::string& scenePath);
        ~Scene() {
            exitScene();
        }
        void enterScene();
        void updateScene(double delta);
        void exitScene();
        void addActiveSystem(System* system, const std::string& name = "");
        void initCamera();
        Camera* getCamera() const { return camera; }
        void setCamera(Camera* cam) { camera = cam; }
        std::vector<System*> getSysthemsByName(const std::string& name) const {
            std::vector<System*> systems;
            for (auto& sys : activeSystems) {
                const std::string sysTypeName = typeid(*sys).name();
                // Try exact mangled name first
                if (sysTypeName == name) {
                    systems.push_back(sys);
                    continue;
                }
                // Fallback: demangle and compare to the requested class name
                int status = 0;
                std::unique_ptr<char, void(*)(void*)> demangled(
                    abi::__cxa_demangle(sysTypeName.c_str(), nullptr, nullptr, &status),
                    std::free);
                if (status == 0 && demangled && name == demangled.get()) {
                    systems.push_back(sys);
                }
            }
            return systems;
        };
        std::vector<System*> getActiveSystems() const { return activeSystems; }
        template <typename T>
        std::vector<T*> getSystemsOfType() const {
            std::vector<T*> systems;
            for (auto* sys : activeSystems) {
                if (auto* casted = dynamic_cast<T*>(sys)) {
                    systems.push_back(casted);
                }
            }
            return systems;
        }
        template <typename T>
        T* getFirstSystemOfType() const {
            for (auto* sys : activeSystems) {
                if (auto* casted = dynamic_cast<T*>(sys)) {
                    return casted;
                }
            }
            return nullptr;
        }
        private:
        std::vector<System*> activeSystems;
        std::string scenePath;
        std::vector<std::string> sceneClasses;
        Camera* camera = nullptr;
    };
}

#endif //NIELS3DGAMEENGINE_SCENE_H

