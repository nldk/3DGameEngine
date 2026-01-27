//
// Created by niel on 1/24/26.
//

#include "phisycs.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include "renderer.h"

Engine::WorldHitbox2D Engine::Hitbox2D::toWorld(const glm::vec3& parentPosition) const {
    float halfW = static_cast<float>(width) * 0.5f;
    float halfH = static_cast<float>(height) * 0.5f;
    glm::vec2 center = glm::vec2(parentPosition.x, parentPosition.y) + offset;
    return {center, glm::vec2(halfW, halfH), interactWithGround};
}

bool Engine::Hitbox2D::overlaps(const WorldHitbox2D& a, const WorldHitbox2D& b) {
    bool overlapX = std::abs(a.center.x - b.center.x) < (a.halfExtents.x + b.halfExtents.x);
    bool overlapY = std::abs(a.center.y - b.center.y) < (a.halfExtents.y + b.halfExtents.y);
    return overlapX && overlapY;
}

std::vector<Engine::WorldHitbox2D> Engine::PhysicsObject2D::getWorldHitboxes() const {
    std::vector<WorldHitbox2D> world;
    if (hitboxes.empty()) {
        float halfW = static_cast<float>(width) * 0.5f;
        float halfH = static_cast<float>(height) * 0.5f;
        world.push_back({glm::vec2(position.x, position.y), glm::vec2(halfW, halfH), true});
        return world;
    }
    world.reserve(hitboxes.size());
    for (const auto& hb : hitboxes) {
        world.push_back(hb.toWorld(position));
    }
    return world;
}

glm::vec2 Engine::PhysicsObject2D::getAggregateHalfExtents() const {
    if (hitboxes.empty()) {
        return glm::vec2(static_cast<float>(width) * 0.5f, static_cast<float>(height) * 0.5f);
    }
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    for (const auto& hb : hitboxes) {
        float halfW = static_cast<float>(hb.width) * 0.5f;
        float halfH = static_cast<float>(hb.height) * 0.5f;
        minX = std::min(minX, hb.offset.x - halfW);
        maxX = std::max(maxX, hb.offset.x + halfW);
        minY = std::min(minY, hb.offset.y - halfH);
        maxY = std::max(maxY, hb.offset.y + halfH);
    }
    return glm::vec2((maxX - minX) * 0.5f, (maxY - minY) * 0.5f);
}

void Engine::PhysicsObject2D::UpdateP(double deltaTime) {
    hasCollidedWithGround = false;
    if (isAffectedByGravity) {
        velocity.y += gravity.y * static_cast<float>(deltaTime);
        if (gravity.y < 0.0f) {
            velocity.y = std::max(velocity.y, terminalVelocity.y); // clamp toward negative terminal when falling down
        } else {
            velocity.y = std::min(velocity.y, terminalVelocity.y);
        }
        // Apply horizontal air friction only
        velocity.x -= velocity.x * AirFriction.x * static_cast<float>(deltaTime);
    }

    glm::vec3 movement = velocity * static_cast<float>(deltaTime);

    glm::vec2 halfExtents = getAggregateHalfExtents();
    float maxMoveX = (halfExtents.x * 2.0f) * 0.9f;
    float maxMoveY = (halfExtents.y * 2.0f) * 0.9f;
    movement.x = glm::clamp(movement.x, -maxMoveX, maxMoveX);
    //movement.y = glm::clamp(movement.y, -maxMoveY, maxMoveY);

    position.x += movement.x;
    for (auto &obj: physicsObjectsToCheck) {
        if (isCollidingAABB(obj)) {
            if (obj->isGround) {
                float resolveShiftX = 0.0f;
                auto thisBoxes = getWorldHitboxes();
                auto otherBoxes = obj->getWorldHitboxes();
                for (const auto& hb : thisBoxes) {
                    if (!hb.interactWithGround) continue;
                    for (const auto& ob : otherBoxes) {
                        if (!Hitbox2D::overlaps(hb, ob)) continue;
                        float dx = hb.center.x - ob.center.x;
                        float overlapX = (hb.halfExtents.x + ob.halfExtents.x) - std::abs(dx);
                        float candidate = dx > 0 ? overlapX : -overlapX;
                        if (std::abs(candidate) > std::abs(resolveShiftX)) {
                            resolveShiftX = candidate;
                        }
                    }
                }
                position.x += resolveShiftX;
                velocity.x = 0.0f;
            } else {
                if (this->onCollision) {
                    this->onCollision(obj);
                }
            }
        }
    }

    position.y += movement.y;
    for (auto &obj: physicsObjectsToCheck) {
        if (isCollidingAABB(obj)) {
            if (obj->isGround) {
                float resolveShiftY = 0.0f;
                bool landed = false;
                auto thisBoxes = getWorldHitboxes();
                auto otherBoxes = obj->getWorldHitboxes();
                const float epsilon = 0.01f;
                bool downIsNegative = gravity.y < 0.0f;

                for (const auto& hb : thisBoxes) {
                    if (!hb.interactWithGround) continue;
                    for (const auto& ob : otherBoxes) {
                        if (!Hitbox2D::overlaps(hb, ob)) continue;
                        float dy = hb.center.y - ob.center.y;
                        float overlapY = (hb.halfExtents.y + ob.halfExtents.y) - std::abs(dy);
                        float candidate = dy > 0 ? overlapY : -overlapY;
                        if (std::abs(candidate) > std::abs(resolveShiftY)) {
                            resolveShiftY = candidate;
                            landed = (downIsNegative && dy > 0) || (!downIsNegative && dy < 0);
                        }
                    }
                }
                if (resolveShiftY != 0.0f) {
                    resolveShiftY += (resolveShiftY > 0 ? epsilon : -epsilon);
                }
                position.y += resolveShiftY;
                hasCollidedWithGround = hasCollidedWithGround || landed;
                velocity.y = 0.0f;
            } else {
                if (this->onCollision) {
                    this->onCollision(obj);
                }
            }
        }
    }
}

bool Engine::PhysicsObject2D::isCollidingAABB(PhysicsObject2D* a) {
    auto thisBoxes = getWorldHitboxes();
    auto otherBoxes = a->getWorldHitboxes();

    for (const auto& hb : thisBoxes) {
        for (const auto& ob : otherBoxes) {
            if (Hitbox2D::overlaps(hb, ob)) {
                return true;
            }
        }
    }
    return false;
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
    // Iterate rows in reverse to match coordinate system (Y increases upward)
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int tileIndex = tilemap[y][x]-1;
            //std::cout << "tileIndex "<<tileIndex << " "<< std::endl;
            //std::cout << "tileIndex > 0:"<< (tileIndex > 0) <<std::endl;
            //std::cout << "tileIndex < textures.size():" << (tileIndex < textures.size()) << std::endl;
            if (tileIndex >= 0 && tileIndex < textures.size()) {
                //std::cout << "adding tile with tileindex "<< tileIndex << std::endl;
                PhysicsSprite2D* tile = new PhysicsSprite2D(GL_NEAREST);
                tile->texture = textures[tileIndex];
                addTile(tile, x, height - 1 - y);
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

void Engine::TileMap2D::addTile(PhysicsSprite2D* tile, int x, int y) {
    //std::cout << "addig tile" << std::endl;
    // Position tiles at their center points (add half width/height offset)
    float centerX = x * tileWidth + tileWidth * 0.5f;
    float centerY = y * tileHeight + tileHeight * 0.5f;
    tile->setPosition(glm::vec2(centerX, centerY));
    tile->scale = glm::vec3(static_cast<float>(tileWidth), static_cast<float>(tileHeight), 1.0f);
    tile->height = tileHeight;
    tile->width = tileWidth;
    tile->setSingleColliderSize(glm::vec2(static_cast<float>(tileWidth), static_cast<float>(tileHeight)));
    tiles.push_back(tile);
}
