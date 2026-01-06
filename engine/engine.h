//
// Created by niel on 1/6/26.
//

#ifndef NIELS3DGAMEENGINE_ENGINE_H
#define NIELS3DGAMEENGINE_ENGINE_H

#include "classRegistarion.h"
#include "utils.h"
#include "window.h"
#include "scene.h"

namespace Engine {
    class Engine {
        public:
        static Engine& Instance(const std::string& path = "") {
            static Engine instance(path);
            return instance;
        }
        ~Engine();
        void changeCurrentScene(std::string path);
        void runUpdateLoop(Window window);
        void exit();
        void start();
        private:
        Engine(const std::string& path);
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        bool running;
        Scene* currentScene;
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
    };
};

#endif //NIELS3DGAMEENGINE_ENGINE_H