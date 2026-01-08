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
    float vertices[32] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
   };
    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    Engine::Renderer* renderer;
    Engine::Texture* texture;
    public:
    TestRenderer() {
        std::cout << "testClass" << std::endl;
        vshader = new Engine::Shader("/home/niel/CLionProjects/Niels3DGameEngine/engine/shaders/shader.v",GL_VERTEX_SHADER,"vertex");
        fshader = new Engine::Shader("/home/niel/CLionProjects/Niels3DGameEngine/engine/shaders/shader.f",GL_FRAGMENT_SHADER,"fragment");
        shaderProgram = new Engine::ShaderProgram();
        shaderProgram->addShader(vshader);
        shaderProgram->addShader(fshader);
        shaderProgram->compile();
        texture = new Engine::Texture("/home/niel/CLionProjects/Niels3DGameEngine/game/assets/container.jpg");
        renderer = new Engine::Renderer(shaderProgram);
        renderer->loadData(vertices,32,indices,6);
        renderer->setVAtributes(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float), 0);
        renderer->setVAtributes(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float), 3 * sizeof(float));
        renderer->setVAtributes(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float), 6 * sizeof(float));
        REGISTER_UPDATE(Update);
    }
    void Update(double deltaTime) {
        //std::cout << "testClass " << deltaTime << std::endl;
        renderer->render(0,6,GL_TRIANGLES,texture);
    }

};
REGISTER_CLASS(TestRenderer);