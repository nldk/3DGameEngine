//
// Created by niel on 1/24/26.
//

#include "phisycs.h"

void Engine::PhysicsObject2D::Update(double deltaTime) {
    if (isAffectedByGravity) {
        velocity.y -= gravity.y * static_cast<float>(deltaTime);
        if (velocity.y < terminalVelocity.y) {
            velocity.y = terminalVelocity.y;
        }
        // Apply air friction
        velocity.x -= velocity.x * AirFriction.x * static_cast<float>(deltaTime);
        velocity.y -= velocity.y * AirFriction.y * static_cast<float>(deltaTime);
    }
    position += velocity * static_cast<float>(deltaTime);
    for (auto &obj: physicsObjectsToCheck) {
        if (isCollidingAABB(obj)) {
            this->onCollision(obj);
        }
    }
}

bool Engine::PhysicsObject2D::isCollidingAABB(PhysicsObject2D* a) {
    return (abs(this->position.x - a->position.x) < (this->width + a->width)) &&
           (abs(this->position.y - a->position.y) < (this->height + a->height));
}