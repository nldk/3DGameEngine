#include <iostream>
#include <ostream>

#include "../engine/engine.h"
class Init : public System {
public:
    Init() {
        WindowStartupConfig::width = 500;
        WindowStartupConfig::height = 500;
        WindowStartupConfig::title = "test";
        WindowStartupConfig::gl_version_major = 3;
        WindowStartupConfig::gl_version_minor = 3;
    }
};
REGISTER_CLASS(Init);

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
        vshader = new Engine::Shader("/home/niel/CLionProjects/Niels3DGameEngine/engine/shaders/shader.v",GL_VERTEX_SHADER,"vertex");
        fshader = new Engine::Shader("/home/niel/CLionProjects/Niels3DGameEngine/engine/shaders/shader.f",GL_FRAGMENT_SHADER,"fragment");
        shaderProgram = new Engine::ShaderProgram();
        shaderProgram->addShader(vshader);
        shaderProgram->addShader(fshader);
        shaderProgram->compile();
        shaderProgram->use();
        texture = new Engine::Texture("/home/niel/CLionProjects/Niels3DGameEngine/game/assets/container.jpg",shaderProgram);
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
REGISTER_CLASS(TestRenderer);