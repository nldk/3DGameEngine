//
// Created by niel on 1/6/26.
//

#ifndef NIELS3DGAMEENGINE_SCENE_H
#define NIELS3DGAMEENGINE_SCENE_H
#include <string>
#include <vector>

#include "classRegistarion.h"

namespace Engine {
    class Scene {
        public:
        Scene(std::string scenePath);
        void enterScene();
        void updateScene(double delta);
        void exitScene();
        private:
        std::vector<System*> activeSystems;
        std::string scenePath;
        std::vector<std::string> sceneClasses;
    };
}

#endif //NIELS3DGAMEENGINE_SCENE_H