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
            if (obj->isGround) {
                if (obj->position.y < position.y + height) {
                    hasCollidedWithGround = true;
                    velocity.y = 0.0f;
                    position.y = obj->position.y - height;
                }
                if (obj->position.x > position.x + width) {
                    velocity.x = 0.0f;
                    position.x = obj->position.x - width;
                }
                if (obj->position.x + obj->width < position.x) {
                    velocity.x = 0.0f;
                    position.x = obj->position.x + obj->width;
                }
            }
            if (this->onCollision) {
                this->onCollision(obj);
            }
        }
    }
}

bool Engine::PhysicsObject2D::isCollidingAABB(PhysicsObject2D* a) {
    return (abs(this->position.x - a->position.x) < (this->width + a->width)) &&
           (abs(this->position.y - a->position.y) < (this->height + a->height));
}

void Engine::TileMap2D::loadTileMap(std::string filePath) {
    std::ifstream file(filePath);
    std::string line;
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }
    std::vector<std::vector<int>> tilemap;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<int> row;

        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));
        }

        tilemap.push_back(row);
    }

    int height = tilemap.size();
    int width = height > 0 ? tilemap[0].size() : 0;
    std::cout << textures.size() << std::endl;
    std::cout << "tilemap:"<< std::endl;
    for (auto & tile : tilemap) {
        for (auto & cell : tile) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileIndex = tilemap[y][x]-1;
            //std::cout << "tileIndex "<<tileIndex << " "<< std::endl;
            //std::cout << "tileIndex > 0:"<< (tileIndex > 0) <<std::endl;
            //std::cout << "tileIndex < textures.size():" << (tileIndex < textures.size()) << std::endl;
            if (tileIndex >= 0 && tileIndex < textures.size()) {
                //std::cout << "adding tile with tileindex "<< tileIndex << std::endl;
                PhisicsSprite2D* tile = new PhisicsSprite2D(GL_NEAREST);
                tile->texture = textures[tileIndex];
                addTile(tile, x, y);
            }
        }
    }

}
void Engine::TileMap2D::Update(double deltaTime) {
    //std::cout << "rendering tilemap with " << tiles.size() << " tiles." << std::endl;
    for (auto & tile : tiles) {
        tile->Render(deltaTime);
    }
}

void Engine::TileMap2D::addTile(PhisicsSprite2D* tile, int x, int y) {
    //std::cout << "addig tile" << std::endl;
    tile->setPosition(glm::vec2(x * tileWidth, y * tileHeight));
    tile->scale = glm::vec3(static_cast<float>(tileWidth), static_cast<float>(tileHeight), 1.0f);
    tiles.push_back(tile);
}
