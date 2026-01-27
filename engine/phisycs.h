//
// Created by niel on 1/24/26.
//

#ifndef NIELS3DGAMEENGINE_PHISYCS_H
#define NIELS3DGAMEENGINE_PHISYCS_H

#include "classRegistarion.h"
#include "engine.h"

namespace Engine {
    // Forward declarations for Texture and PhisicsSprite2D
    class Texture;
    class PhysicsSprite2D;
    // Forward declaration for Sprite2D
    class Sprite2D;
    struct WorldHitbox2D {
        glm::vec2 center;
        glm::vec2 halfExtents;
        bool interactWithGround;
    };
    class Hitbox2D {
    public:
        int width, height;
        glm::vec2 offset; // offset from parent PhysicsObject2D center
        bool interactWithGround;
        Hitbox2D(int width, int height, glm::vec2 offset = glm::vec2(0.0f), bool interactWithGround = true)
                : width(width), height(height), offset(offset), interactWithGround(interactWithGround) {};
        WorldHitbox2D toWorld(const glm::vec3& parentPosition) const;
        static bool overlaps(const WorldHitbox2D& a, const WorldHitbox2D& b);
    };
    class PhysicsObject2D: virtual public System {
    public:
        bool isGround = false;
        bool hasCollidedWithGround = false;
        int width, height;
        glm::vec3 position;
        glm::vec3 velocity;
        std::vector<Hitbox2D> hitboxes;
        std::vector<PhysicsObject2D*> physicsObjectsToCheck;
        std::function<void(PhysicsObject2D* colObj)> onCollision = [](PhysicsObject2D*) {};
        PhysicsObject2D(int width, int height, glm::vec3 position) : width(width), height(height), position(position), velocity(glm::vec3(0.0f)) {
            REGISTER_UPDATE(UpdateP);
        }
        bool isCollidingAABB(PhysicsObject2D* a);
        void UpdateP(double deltaTime);
        void addHitbox(int hitboxWidth, int hitboxHeight, glm::vec2 offset = glm::vec2(0.0f), bool interactWithGround = true) {
            hitboxes.emplace_back(hitboxWidth, hitboxHeight, offset, interactWithGround);
        }
        void addHitbox(const Hitbox2D& hb) {
            hitboxes.push_back(hb);
        }
        void clearHitboxes() {
            hitboxes.clear();
        }
        void setSingleColliderSize(const glm::vec2& size) {
            width = static_cast<int>(size.x);
            height = static_cast<int>(size.y);
            hitboxes.clear();
            hitboxes.emplace_back(width, height, glm::vec2(0.0f), true);
        }
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
        void setFallGravityMultiplier(float multiplier) {
            fallGravityMultiplier = multiplier;
        }
        glm::vec2 getGravity() const { return gravity; }
        std::vector<WorldHitbox2D> getWorldHitboxes() const;
    private:
        bool isAffectedByGravity = false;
        glm::vec2 gravity = glm::vec2(0.0f, -200.0f);              // -Y is downward
        glm::vec2 terminalVelocity = glm::vec2(0.0f, -500.0f);    // Clamp toward negative Y when falling
        glm::vec2 AirFriction = glm::vec2(5.0f, 0.0f);
        float fallGravityMultiplier = 1.0f;                        // Extra gravity when falling down
        glm::vec2 getAggregateHalfExtents() const;
    };
    class TileMap2D: public System {
    public:
        int tileWidth, tileHeight;
        std::vector<PhysicsObject2D*> groundTiles;
        std::vector<Texture*> textures;
        std::vector<PhysicsSprite2D*> tiles;
        TileMap2D(int tileWidth, int tileHeight) : tileWidth(tileWidth), tileHeight(tileHeight) {
            REGISTER_UPDATE(Update);
            std::cout << tileWidth << " " << tileHeight << std::endl;
        };
        void addTileToGround(PhysicsObject2D* tile, int x, int y) {
            tile->position = glm::vec3(x * tileWidth, y * tileHeight, 0.0f);
            groundTiles.push_back(tile);
        }
        void addTexture(Texture* texture) {
            textures.push_back(texture);
        }
        void addTile(PhysicsSprite2D* tile, int x, int y);
        void loadTileMap(std::string filePath);
        void Update(double deltaTime);
    };
}

// Remove renderer include and PhysicsSprite2D definition from this header to avoid circular dependency

#endif //NIELS3DGAMEENGINE_PHISYCS_H

