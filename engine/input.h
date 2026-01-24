//
// Created by niel on 1/24/26.
//

#ifndef NIELS3DGAMEENGINE_INPUT_H
#define NIELS3DGAMEENGINE_INPUT_H
#include "engine.h"
namespace Engine {
    inline bool isKeyPressed(int key) {
        return Engine::Engine::Instance().window->isKeyPressed(key);
    }
    inline bool isKeyReleased(int key) {
        return Engine::Engine::Instance().window->isKeyReleased(key);
    }
    inline glm::vec2 getMousePosition() {
        return Engine::Engine::Instance().window->getMousePosition();
    }
    inline glm::vec2 getMouseScrollOffset() {
        return Engine::Engine::Instance().window->getMouseScrollOffset();
    }
    inline bool isMouseButtonPressed(int button) {
        return Engine::Engine::Instance().window->isMouseButtonPressed(button);
    }
}

#endif //NIELS3DGAMEENGINE_INPUT_H