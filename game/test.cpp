#include <iostream>
#include <ostream>

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
        std::cout << "testClass" << std::endl;
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
        std::cout << "testClass " << deltaTime << std::endl;
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
            setColliderSize(glm::vec2(75.0f, 100.0f));
            rotation = 0.0f;
            setIsAffectedByGravity(true);
            setGravity(glm::vec2(0.0f, -1000.0f));
            setTerminalVelocity(glm::vec2(0.0f, -1200.0f));
            setFallGravityMultiplier(2.5f);  // Apply 2.5x gravity when falling for shorter jumps
            setTerminalVelocity(glm::vec2(0.0f, -10000.0f));
            hitboxes[0].offset = glm::vec2(0.0f, -13.0f);
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

        //std::cout << this << std::endl;
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
            std::cout << "killZone collided with object @" << colObj << std::endl;
            Player* player = dynamic_cast<Player*>(colObj);
            if (player) {
                Engine::Engine::Instance().changeCurrentScene("game/testScene.scene");
            }
        };
        physicsObjectsToCheck.push_back(dynamic_cast<PhysicsObject2D *>(Engine::Engine::Instance().getCurrentScene()->getSysthemsByName("Player")[0]));
    }
};
REGISTER_CLASS(killZone);