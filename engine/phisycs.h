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
    class PhisicsSprite2D;
    // Forward declaration for Sprite2D
    class Sprite2D;

    class PhysicsObject2D: virtual public System {
    public:
        bool isGround = false;
        bool hasCollidedWithGround = false;
        int width, height;
        glm::vec3 position;
        glm::vec3 velocity;
        std::vector<PhysicsObject2D*> physicsObjectsToCheck;
        std::function<void(PhysicsObject2D* colObj)> onCollision = [](PhysicsObject2D*) {};
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
    class TileMap2D: public System {
    public:
        int tileWidth, tileHeight;
        std::vector<PhysicsObject2D*> groundTiles;
        std::vector<Texture*> textures;
        std::vector<PhisicsSprite2D*> tiles;
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
        void addTile(PhisicsSprite2D* tile, int x, int y);
        void loadTileMap(std::string filePath);
        void Update(double deltaTime);
    };
}

// Include renderer.h here after class definitions to avoid circular dependency
#include "renderer.h"

namespace Engine {
    class PhisicsSprite2D : public PhysicsObject2D, public Sprite2D {
    public:
        PhisicsSprite2D(std::string filePath, GLenum sampleMode) : PhysicsObject2D(0,0,glm::vec3(0.0f)), Sprite2D(filePath,sampleMode) {
            width = texture ? texture->width : 0;
            height = texture ? texture->height : 0;
            REGISTER_UPDATE(Update);
        }
        PhisicsSprite2D(Texture* tex, GLenum sampleMode): PhysicsObject2D(0,0,glm::vec3(0.0f)), Sprite2D(sampleMode) {
            setTexture(tex);
            width = tex ? tex->width : 0;
            height = tex ? tex->height : 0;
            REGISTER_UPDATE(Update);
        }
        PhisicsSprite2D(GLenum sampleMode): PhysicsObject2D(0,0,glm::vec3(0.0f)), Sprite2D(sampleMode) {
            width = 0;
            height = 0;
            REGISTER_UPDATE(Update);
        };
        void Update(double deltaTime) {
            Sprite2D::position = PhysicsObject2D::position;
        }
        void setPosition(glm::vec2 newPosition) {
            PhysicsObject2D::position = glm::vec3(newPosition, 0.0f);
            Sprite2D::position = glm::vec3(newPosition, 0.0f);
        }
        glm::vec2 getPositionOfP() {
            return glm::vec2(PhysicsObject2D::position.x, PhysicsObject2D::position.y);
        }
        glm::vec2 getPositionOfS() {
            return glm::vec2(Sprite2D::position.x, Sprite2D::position.y);
        }
    };
}

#endif //NIELS3DGAMEENGINE_PHISYCS_H