//
// Created by niel on 1/6/26.
//

#ifndef NIELS3DGAMEENGINE_SCENE_H
#define NIELS3DGAMEENGINE_SCENE_H
#include <string>
#include <vector>

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
        void addActiveSystem(System* system) {
            activeSystems.push_back(system);
        }
        void initCamera();
        Camera* getCamera() const { return camera; }
        void setCamera(Camera* cam) { camera = cam; }
        private:
        std::vector<System*> activeSystems;
        std::string scenePath;
        std::vector<std::string> sceneClasses;
        Camera* camera = nullptr;
    };
}

#endif //NIELS3DGAMEENGINE_SCENE_H

