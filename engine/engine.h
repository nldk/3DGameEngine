//
// Created by niel on 1/6/26.
//

#ifndef NIELS3DGAMEENGINE_ENGINE_H
#define NIELS3DGAMEENGINE_ENGINE_H

#include "classRegistarion.h"
#include <physfs.h>
#include <filesystem>

inline std::string GetGamePath(const char* argv0 = nullptr) {
    namespace fs = std::filesystem;

    if (argv0) {
        try {
            // Resolve the executable path from argv[0]
            fs::path exePath = fs::absolute(argv0);

            // Try: executable_dir/../game/
            fs::path gamePathCandidate = exePath.parent_path().parent_path() / "game";
            if (fs::exists(gamePathCandidate)) {
                return gamePathCandidate.string() + "/";
            }

            // Fallback: executable_dir/game/
            gamePathCandidate = exePath.parent_path() / "game";
            if (fs::exists(gamePathCandidate)) {
                return gamePathCandidate.string() + "/";
            }
        } catch (...) {
            // If anything fails, fall through to relative path
        }
    }

    // Final fallback: relative path (for cases like CLion)
    return "game/";
}

// Declared here, defined in Entry.cpp
extern std::string gamePath;

#include "utils.h"
#include "window.h"
#include "scene.h"
#include "stb_image.h"
#include "glm-1.0.3/glm/glm.hpp"
#include "glm-1.0.3/glm/gtc/matrix_transform.hpp"
#include "glm-1.0.3/glm/gtc/type_ptr.hpp"
#include "soundEngine.h"

namespace Engine {
    class ShaderProgram;
    class Renderer;
    class DebugRenderer2D;
    class Engine {
        public:
        ShaderProgram* defaultShaderProgram;
        Renderer* SpriteRenderer;
        DebugRenderer2D* debugHitboxRenderer;
        Window* window;
        static Engine& Instance() {
            static Engine instance("");
            return instance;
        }
        static void Initialize(const std::string& path) {
            Instance().changeCurrentScene(path);
        }
        Scene* getCurrentScene() const { return currentScene; }
        DebugRenderer2D* getDebugRenderer2D() const { return debugHitboxRenderer; }
        void setDebugDrawHitboxes(bool enabled) { debugDrawHitboxes = enabled; }
        bool getDebugDrawHitboxes() const { return debugDrawHitboxes; }
        ~Engine();
        void changeCurrentScene(const std::string& path);
        void runUpdateLoop();
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
        bool debugDrawHitboxes = false;
        bool sceneChangeRequested = false;
        std::string pendingScenePath;
    };

};
#include "input.h"
#define CREATE_AND_REGISTER(VAR_NAME) \
    Engine::Engine::Instance().getCurrentScene()->addActiveSystem(VAR_NAME);
#endif //NIELS3DGAMEENGINE_ENGINE_H