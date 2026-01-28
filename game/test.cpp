#include <iostream>
#include <ostream>
#include <random>
#include <unordered_map>
#include <algorithm>

#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../engine/phisycs.h"
#include "engine/soundEngine.h"


class Init : public System {
public:
    Init() {
        WindowStartupConfig::width = 1000;
        WindowStartupConfig::height = 1000;
        WindowStartupConfig::title = "test";
        WindowStartupConfig::gl_version_major = 3;
        WindowStartupConfig::gl_version_minor = 3;
        WindowStartupConfig::vSync = false;
    }
};
REGISTER_CLASS(Init);
/*
class SceneCamera : public Engine::Camera {
public:
    SceneCamera() : Engine::Camera(Engine::Engine::Instance().defaultShaderProgram) {
        Engine::Scene* scene = Engine::Engine::Instance().getCurrentScene();
        if (scene) {
            scene->setCamera(this);
        }
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        zoom = 1.0f;
    }
};
REGISTER_CLASS(SceneCamera);

class TestRenderer : public System {
    Engine::Shader* vshader;
    Engine::Shader* fshader;
    Engine::ShaderProgram* shaderProgram;
    float vertices[180] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    Engine::Renderer* renderer;
    Engine::Texture* texture;
    glm::mat4 trans;
    float rotationAngle = 0.0f;
    public:
    TestRenderer() {
#ifndef NDEBUG
        std::cout << "testClass" << std::endl;
#endif
        trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(1, 1, 0.5));
        vshader = new Engine::Shader("engine/shaders/shader.v",GL_VERTEX_SHADER,"vertex");
        fshader = new Engine::Shader("engine/shaders/shader.f",GL_FRAGMENT_SHADER,"fragment");
        shaderProgram = new Engine::ShaderProgram();
        shaderProgram->addShader(vshader);
        shaderProgram->addShader(fshader);
        shaderProgram->compile();
        shaderProgram->use();
        texture = new Engine::Texture("game/assets/container.jpg",shaderProgram);
        renderer = new Engine::Renderer(shaderProgram);
        renderer->loadData(vertices,180,indices,6);
        renderer->setVAtributes(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float), 0);
        renderer->setVAtributes(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float), 3 * sizeof(float));
        REGISTER_UPDATE(Update);
    }
    void Update(double deltaTime) {
#ifndef NDEBUG
        std::cout << "testClass " << deltaTime << std::endl;
#endif
        trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
        trans = glm::rotate(trans, glm::radians(rotationAngle), glm::vec3(0.0, 0.0, 1.0));
        renderer->renderUDVA(0,36,GL_TRIANGLES,texture,trans);
        rotationAngle += 50.0f * static_cast<float>(deltaTime);
    }

};
//REGISTER_CLASS(TestRenderer);
*/
class TestTilemap : public Engine::TileMap2D {
public:
    TestTilemap() : Engine::TileMap2D(64,64) {
        Engine::Texture* tileTexture = new Engine::Texture("game/assets/terrain_dirt_cloud_left.png",GL_NEAREST);
        addTexture(tileTexture);
        Engine::Texture* tileTexture2 = new Engine::Texture("game/assets/terrain_dirt_cloud_middle.png",GL_NEAREST);
        addTexture(tileTexture2);
        Engine::Texture* tileTexture3 = new Engine::Texture("game/assets/terrain_dirt_cloud_right.png",GL_NEAREST);
        addTexture(tileTexture3);
        loadTileMap("game/tilemap.tilemap");
        for (auto &tile : tiles) {
            tile->isGround = true;
        }
    }
};
REGISTER_CLASS(TestTilemap);
class Player : public Engine::AnimatedPyshicsSprite2D {
public:
    Engine::Camera* camera;
    Engine::TileMap2D* tilemap = nullptr;
    bool physicsBound = false;
    Player() : AnimatedPyshicsSprite2D(GL_NEAREST) {
            //std::cout << "Player ctor @" << this << std::endl;
            Engine::Animation2D* run = new Engine::Animation2D(0.15f);
            run->addFrame(new Engine::Texture("game/assets/character_pink_walk_a.png"));
            run->addFrame(new Engine::Texture("game/assets/character_pink_walk_b.png"));
            run->frameDuration = 0.20f;
            addAnimation("run", run);
            Engine::Animation2D* idle = new Engine::Animation2D(0.25f);
            idle->addFrame(new Engine::Texture("game/assets/character_pink_front.png",GL_NEAREST));
            addAnimation("idle", idle);
            playAnimation("idle");
            if (idle->getCurrentFrame()) {
                setTexture(idle->getCurrentFrame());
            }
            // Collision and physics bindings are resolved lazily once the scene is fully constructed
            setPosition(glm::vec2(200.0f, 200.0f));
            PhysicsObject2D::position.z = -0.1f;
            Sprite2D::setZForOrdering(-0.1f);
            setScale(glm::vec3(128.0f, 128.0f, 1.0f));
            setColliderSize(glm::vec2(75.0f, 90.0f));
            rotation = 0.0f;
            setIsAffectedByGravity(true);
            setGravity(glm::vec2(0.0f, -1000.0f));
            setTerminalVelocity(glm::vec2(0.0f, -1200.0f));
            setFallGravityMultiplier(2.5f);  // Apply 2.5x gravity when falling for shorter jumps
            setTerminalVelocity(glm::vec2(0.0f, -10000.0f));
            hitboxes[0].offset = glm::vec2(0.0f, -15.0f);
            hitboxes[0].interactWithGround = true;
            REGISTER_UPDATE(Update);
            camera = Engine::Engine::Instance().getCurrentScene()->getCamera();
            auto regs = Engine::Engine::Instance().getCurrentScene()->getSysthemsByName("TestTilemap");
            for (auto& sys : regs) {
                tilemap = dynamic_cast<Engine::TileMap2D*>(sys);
                for (auto & tile : tilemap->tiles) {
                    physicsObjectsToCheck.push_back(tile);
                }
            }
        }

     void Update(double delta) {
        std::cout << 1/delta << " FPS" << std::endl;

         if (Engine::isKeyPressed(GLFW_KEY_SPACE) && hasCollidedWithGround) {
             velocity.y = 750.0f;  // Reduced from 700.0f for shorter jumps
         }
         if (Engine::isKeyPressed(GLFW_KEY_A)) {
             velocity.x = -300.0f;
             playAnimation("run");
             setFlip(true,false);
         } else if (Engine::isKeyPressed(GLFW_KEY_D)) {
             velocity.x = 300.0f;
             playAnimation("run");
             setFlip(false,false);
         } else {
             velocity.x = 0.0f;
             playAnimation("idle");
         }

        camera->setPosition(glm::vec3(getPositionOfS(),0.0f));
    }
};
REGISTER_CLASS(Player);

class AudioTest: public Engine::AudioPlayerG {
public:
    AudioTest() : Engine::AudioPlayerG("game/assets/music.mp3", false) {
        playSound(2);
        REGISTER_UPDATE(Update);
    }
};
REGISTER_CLASS(AudioTest);

class killZone: public Engine::PhysicsObject2D {
public:
    killZone() : Engine::PhysicsObject2D(800,50,glm::vec3(300.0f, -100.0f, 0.0f)) {
        setSingleColliderSize(glm::vec2(5000.0f,50.0f));
        isGround = false;
        onCollision = [this](Engine::PhysicsObject2D* colObj) {
#ifndef NDEBUG
            std::cout << "killZone collided with object @" << colObj << std::endl;
#endif
            Player* player = dynamic_cast<Player*>(colObj);
            auto* scene = Engine::Engine::Instance().getCurrentScene();
            if (player) {
                Engine::Engine::Instance().changeCurrentScene("game/testScene.scene");
            } else if (scene) {
                scene->removeSystem(colObj);
            }
        };
        physicsObjectsToCheck.push_back(dynamic_cast<PhysicsObject2D *>(Engine::Engine::Instance().getCurrentScene()->getSysthemsByName("Player")[0]));
    }
};
REGISTER_CLASS(killZone);

class Enemy : public Engine::PhisicsSprite2D {
public:
    glm::vec2 moveDir{1.0f, 0.0f};
    float patrolSpeed = 100.0f;
    Engine::PhysicsObject2D* left = nullptr;
    Engine::PhysicsObject2D* right = nullptr;
    Engine::PhysicsObject2D* killsZoneForEnemy = nullptr;
    bool isDead = false;
    void handleStomp(Player* player) {
        if (isDead || !player) return;
        isDead = true;
        player->velocity.y = 600.0f; // bounce the player upward after stomp
        auto* scene = Engine::Engine::Instance().getCurrentScene();
        if (!scene) return;
        scene->removeSystem(this);
        scene->removeSystem(killsZoneForEnemy);
        scene->removeSystem(left);
        scene->removeSystem(right);
    }
    Enemy() : Engine::PhisicsSprite2D("game/assets/block_spikes.png",GL_NEAREST) {
        setPosition(glm::vec2(500.0f, 200.0f));
        setScale(glm::vec3(64.0f, 64.0f,1.0f));
        setSingleColliderSize(glm::vec2(40.0f,40.0f));
        setIsAffectedByGravity(true);
        setGravity(glm::vec2(0.0f, -1000.0f));
        setTerminalVelocity(glm::vec2(0.0f, -1200.0f));
        hitboxes[0].offset = glm::vec2(0.0f, 0.0f);
        hitboxes[0].interactWithGround = true;
        REGISTER_UPDATE(Update);
        left = new Engine::PhysicsObject2D(10,10,glm::vec3(getPositionOfP(),0.0f));
        left->setSingleColliderSize(glm::vec2(25.0f,20.0f));
        left->hitboxes[0].interactWithGround = true;
        left->setIsAffectedByGravity(false);
        right = new Engine::PhysicsObject2D(10,10,glm::vec3(getPositionOfP(),0.0f));
        right->setSingleColliderSize(glm::vec2(25.0f,30.0f));
        right->hitboxes[0].interactWithGround = true;
        right->setIsAffectedByGravity(false);
        auto tilemap = dynamic_cast<Engine::TileMap2D*>(Engine::Engine::Instance().getCurrentScene()->getSysthemsByName("TestTilemap")[0]);
        for (auto & tile : tilemap->tiles) {
            physicsObjectsToCheck.push_back(tile);
            left->physicsObjectsToCheck.push_back(tile);
            right->physicsObjectsToCheck.push_back(tile);
        }
        auto player = dynamic_cast<Engine::PhysicsObject2D*>(Engine::Engine::Instance().getCurrentScene()->getSysthemsByName("Player")[0]);
        physicsObjectsToCheck.push_back(player);
        onCollision = [this](Engine::PhysicsObject2D* colObj) {
            auto* player = dynamic_cast<Player*>(colObj);
            if (!player) return;
            const glm::vec2 playerPos = player->getPositionOfP();
            const glm::vec2 enemyPos = this->getPositionOfP();
            const bool playerAbove = playerPos.y > enemyPos.y;
            const bool playerDescending = player->velocity.y <= 0.0f;
            if (playerAbove && playerDescending) {
                handleStomp(player);
            } else {
                Engine::Engine::Instance().changeCurrentScene("game/testScene.scene");
            }
        };
        killsZoneForEnemy = new Engine::PhysicsObject2D(10,10,glm::vec3(getPositionOfP() + glm::vec2(0.0f,100.0f), 0.0f));
        killsZoneForEnemy->physicsObjectsToCheck.push_back(player);
        killsZoneForEnemy->setSingleColliderSize(glm::vec2(40.0f,10.0f));
        killsZoneForEnemy->onCollision = [this](Engine::PhysicsObject2D* colObj) {
            auto* player = dynamic_cast<Player*>(colObj);
            if (!player) return;
            // Only count as a stomp if the player is above and moving downward.
            const glm::vec2 playerPos = player->getPositionOfP();
            const glm::vec2 enemyPos = this->getPositionOfP();
            const bool playerAbove = playerPos.y > enemyPos.y;
            const bool playerDescending = player->velocity.y <= 0.0f;
            if (playerAbove && playerDescending) {
                handleStomp(player);
            }
        };
        CREATE_AND_REGISTER(killsZoneForEnemy);
        CREATE_AND_REGISTER(left);
        CREATE_AND_REGISTER(right);
    }
    void Update(double delta) {
        // Position side probes near the enemy’s feet before running their physics.
        killsZoneForEnemy->position = glm::vec3(getPositionOfP() + glm::vec2(0.0f,30.0f), 0.0f);
        const glm::vec2 colliderSize = getColliderSize();
        const float halfW = colliderSize.x * 0.5f;
        const float halfH = colliderSize.y * 0.5f;
        const glm::vec2 basePos = getPositionOfP();
        const float footProbeY = basePos.y - halfH - 4.0f;
        left->position = glm::vec3(basePos.x - halfW - 6.0f, footProbeY, 0.0f);
        right->position = glm::vec3(basePos.x + halfW + 6.0f, footProbeY, 0.0f);
        left->velocity = glm::vec3(0.0f);
        right->velocity = glm::vec3(0.0f);
        //left->UpdateP(delta);
        //right->UpdateP(delta);

        const bool groundLeft = left->hasCollidedWithGround;
        const bool groundRight = right->hasCollidedWithGround;

        if (!groundRight) {
            moveDir.x = -1.0f;
            setFlip(true,false);
        } else if (!groundLeft) {
            moveDir.x = 1.0f;
            setFlip(false,false);
        }

        velocity.x = moveDir.x * patrolSpeed;
    }
};
REGISTER_CLASS(Enemy);

class EnemySpawner: public System {
public:
    std::vector<glm::vec2> spawnPoints;
    EnemySpawner() {
        auto* scene = Engine::Engine::Instance().getCurrentScene();
        if (!scene) return;
        auto maps = scene->getSysthemsByName("TestTilemap");
        if (maps.empty()) return;
        auto* tilemap = dynamic_cast<Engine::TileMap2D*>(maps.front());
        if (!tilemap) return;

        const float tileW = static_cast<float>(tilemap->tileWidth);
        const float tileH = static_cast<float>(tilemap->tileHeight);
        // Bucket ground tiles by row, using grid coords derived from tile centers.
        std::unordered_map<int, std::vector<int>> rows;
        rows.reserve(tilemap->tiles.size());
        for (auto* tile : tilemap->tiles) {
            const glm::vec2 tilePos = tile->getPositionOfP();
            int gx = static_cast<int>(std::round((tilePos.x - tileW * 0.5f) / tileW));
            int gy = static_cast<int>(std::round((tilePos.y - tileH * 0.5f) / tileH));
            rows[gy].push_back(gx);
        }

        // For each row, group contiguous x indices into platforms and pick one spawn at the platform mid.
        for (auto& [gy, xs] : rows) {
            std::sort(xs.begin(), xs.end());
            xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
            if (xs.empty()) continue;
            int start = xs.front();
            int prev = start;
            for (size_t i = 1; i <= xs.size(); ++i) {
                bool endOfRun = (i == xs.size()) || (xs[i] != prev + 1);
                if (endOfRun) {
                    int end = prev;
                    float spawnX = (static_cast<float>(start + end + 1) * tileW) * 0.5f;
                    const float tileCenterY = gy * tileH + tileH * 0.5f;
                    const float enemyHalfH = 20.0f; // enemy collider half height
                    float spawnY = tileCenterY + tileH * 0.5f + enemyHalfH + 2.0f; // sit on top of tile
                    spawnPoints.emplace_back(spawnX, spawnY);
                    if (i < xs.size()) {
                        start = xs[i];
                    }
                }
                if (i < xs.size()) {
                    prev = xs[i];
                }
            }
        }

        for (const auto& pos : spawnPoints) {
            auto* enemy = new Enemy();
            enemy->setPosition(pos);
            CREATE_AND_REGISTER(enemy);
        }
    }
};
REGISTER_CLASS(EnemySpawner);

class FinishGoal : public Engine::PhisicsSprite2D {
public:
    bool triggered = false;
    FinishGoal() : Engine::PhisicsSprite2D("game/assets/block_blue.png", GL_NEAREST) {
        auto* scene = Engine::Engine::Instance().getCurrentScene();
        auto maps = scene ? scene->getSysthemsByName("TestTilemap") : std::vector<System*>();
        auto* tilemap = (!maps.empty()) ? dynamic_cast<Engine::TileMap2D*>(maps.front()) : nullptr;
        glm::vec2 pos(300.0f, 150.0f);
        if (tilemap && !tilemap->tiles.empty()) {
            float maxX = tilemap->tiles.front()->getPositionOfP().x;
            float topY = tilemap->tiles.front()->getPositionOfP().y;
            for (auto* tile : tilemap->tiles) {
                const glm::vec2 tpos = tile->getPositionOfP();
                if (tpos.x > maxX) {
                    maxX = tpos.x;
                    topY = tpos.y;
                }
            }
            pos.x = maxX + tilemap->tileWidth; // place just to the right of the last tile
            pos.y = topY + tilemap->tileHeight * 0.5f;
        }
        setPosition(pos);
        setScale(glm::vec3(64.0f, 128.0f, 1.0f));
        setSingleColliderSize(glm::vec2(40.0f, 100.0f));
        setIsAffectedByGravity(false);
        onCollision = [this](Engine::PhysicsObject2D* colObj) {
            if (triggered) return;
            auto* player = dynamic_cast<Player*>(colObj);
            if (!player) return;
            triggered = true;
            Engine::Engine::Instance().changeCurrentScene("game/testScene.scene");
        };
    }
};
REGISTER_CLASS(FinishGoal);
