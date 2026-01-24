//
// Created by niel on 1/24/26.
//

#ifndef NIELS3DGAMEENGINE_PHISYCS_H
#define NIELS3DGAMEENGINE_PHISYCS_H

#include "classRegistarion.h"
#include "engine.h"
namespace Engine {
    class PhysicsObject2D: virtual public System {
    public:
        int width, height;
        glm::vec3 position;
        glm::vec3 velocity;
        std::vector<PhysicsObject2D*> physicsObjectsToCheck;
        std::function<void(PhysicsObject2D* colObj)> onCollision;
        PhysicsObject2D(int width, int height, glm::vec3 position) : width(width), height(height), position(position), velocity(glm::vec3(0.0f)) {
            REGISTER_UPDATE(Update);
        }
        bool isCollidingAABB(PhysicsObject2D* a);
        void Update(double deltaTime);
        void setIsAffectedByGravity(bool affected) {
            isAffectedByGravity = affected;
        }
        void setGravity(const glm::vec2& grav) {
            gravity = grav;
        }
        void setTerminalVelocity(const glm::vec2& termVel) {
            terminalVelocity = termVel;
        }
        void setAirFriction(const glm::vec2& airFric) {
            AirFriction = airFric;
        }
    private:
        bool isAffectedByGravity = false;
        glm::vec2 gravity = glm::vec2(0.0f, 98.1f);
        glm::vec2 terminalVelocity = glm::vec2(0.0f, -500.0f);
        glm::vec2 AirFriction = glm::vec2(0.1f, 0.1f);
    };
}

#endif //NIELS3DGAMEENGINE_PHISYCS_H