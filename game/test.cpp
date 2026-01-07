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
    float vertices[12] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left
   };
    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    Engine::Renderer* renderer;
    public:
    TestRenderer() {
        std::cout << "testClass" << std::endl;
        vshader = new Engine::Shader("/home/niel/CLionProjects/Niels3DGameEngine/engine/shaders/shader.v",GL_VERTEX_SHADER,"vertex");
        fshader = new Engine::Shader("/home/niel/CLionProjects/Niels3DGameEngine/engine/shaders/shader.f",GL_FRAGMENT_SHADER,"fragment");
        shaderProgram = new Engine::ShaderProgram();
        shaderProgram->addShader(vshader);
        shaderProgram->addShader(fshader);
        shaderProgram->compile();
        renderer = new Engine::Renderer(shaderProgram);
        renderer->loadData(vertices,12,indices,6);
        renderer->setVAtributes(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float), 0);
        REGISTER_UPDATE(Update);
    }
    void Update(double deltaTime) {
        std::cout << "testClass " << deltaTime << std::endl;
        renderer->render(0,6,GL_TRIANGLES);
    }

};
REGISTER_CLASS(TestRenderer);