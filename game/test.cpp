#include <iostream>
#include <ostream>

#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../engine/phisycs.h"
#include "engine/soundEngine.h"

class Init : public System {
public:
    Init() {
        WindowStartupConfig::width = 500;
        WindowStartupConfig::height = 500;
        WindowStartupConfig::title = "test";
        WindowStartupConfig::gl_version_major = 3;
        WindowStartupConfig::gl_version_minor = 3;
        WindowStartupConfig::vSync = true;
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
class Player : public Engine::PhisicsSprite2D {
public:
    Engine::Camera* camera;
    Engine::TileMap2D* tilemap = nullptr;
        Player() : PhisicsSprite2D("game/assets/character_pink_front.png",GL_NEAREST) {
        auto regs = Engine::getRegisteredClasses("TestTilemap");
        if (!regs.empty() && regs[0] != nullptr) {
            tilemap = dynamic_cast<Engine::TileMap2D*>(regs[0]);
        } else {
            std::cerr << "Error: TestTilemap not registered or factory failed" << std::endl;
        }
        if (tilemap) {
            for (auto* tile : tilemap->tiles) {
                if (tile) {
                    physicsObjectsToCheck.push_back(tile);
                }
            }
        } else {
            std::cerr << "Error: TestTilemap is not a TileMap2D" << std::endl;
        }
         setPosition(glm::vec2(0.0f,0.0f));
         scale = glm::vec3(100.0f, 100.0f, 1.0f);
         rotation = 0.0f;
         setIsAffectedByGravity(true);
         REGISTER_UPDATE(Update);
         camera = Engine::Engine::Instance().getCurrentScene()->getCamera();
     }
    void Update(double delta) {
        //std::cout << "SpritePos " <<Sprite2D::position.x << " "<< Sprite2D::position.y << std::endl;
        //std::cout << "PhysicsObject pos"<<PhysicsObject2D::position.x << " "<< PhysicsObject2D::position.y << std::endl;
        if (Engine::isKeyPressed(GLFW_KEY_SPACE) && velocity.y <= 0.0f) {
            velocity.y = 100.0f;
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
