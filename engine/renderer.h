//
// Created by niel on 1/7/26.
//

#ifndef NIELS3DGAMEENGINE_RENDERER_H
#define NIELS3DGAMEENGINE_RENDERER_H

#include <glad/glad.h>
#include "engine.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "glm-1.0.3/glm/glm.hpp"
#include  "glm-1.0.3/glm/gtc/matrix_transform.hpp"
#include "glm-1.0.3/glm/gtc/type_ptr.hpp"

namespace Engine {
    class Texture;
    class Shader {
    public:
        unsigned int shader;
        GLenum shaderType;
        std::string name;
        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        Shader(const char *shaderPath, int shaderType, std::string name);

        ~Shader() {
            glDeleteShader(shader);
        }

    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkCompileErrors(unsigned int shader, std::string type);
    };

    class ShaderProgram {
    public:
        unsigned int ID;
        std::vector<Shader *> shaders;

        ShaderProgram() {
            ID = glCreateProgram();
        };

        void addShader(Shader *shader);

        void compile();

        void use();
        void setBool(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
        }

        // ------------------------------------------------------------------------
        void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setUniformMatrix4fv(const std::string &name, glm::mat4 value) {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        };
        // utility uniform functions
        // ------------------------------------------------------------------------

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
    };

    class Renderer {
        public:
        ShaderProgram* shaderProgram;
        Renderer(ShaderProgram* shaderProgram);
        void loadData(float *vertex, int vertexCount, unsigned int* indices, int indexCount);
        void setVAtributes(int layout, unsigned int size, GLenum type, GLboolean normalize, unsigned int stride, unsigned int offset);
        void render(unsigned int start, unsigned int count,GLenum type,Texture* tex,glm::mat4 model);
        void renderUDVA(unsigned int start, unsigned int count,GLenum type,Texture* tex,glm::mat4 model);
        private:
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
    };

    class Texture {
        public:
        unsigned int ID;
        int width, height, nrChannels;
        unsigned char *data;
        GLenum sampleMode = GL_LINEAR;
        void bind() const {
            glBindTexture(GL_TEXTURE_2D, ID);
        }
        Texture(std::string filePath, ShaderProgram* shaderProgram, GLenum sampleMode);
        Texture(std::string filePath, ShaderProgram* shaderProgram);
        ~Texture();
    };
    inline std::vector<float> generateTextureVertices(float width, float height) {
        // Normalize to 1x1 units regardless of texture dimensions
        // This allows scale to control actual size in pixels
        return {
            // posities              // texture coords
             0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // rechtsboven
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // rechtsonder
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // linksonder
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // linksboven
        };
    }
    
    inline std::vector<unsigned int> generateTextureIndices() {
        return {
            0, 1, 3,   // eerste driehoek
            1, 2, 3    // tweede driehoek
        };
    }
    
    class Sprite2D : virtual public System {
        public:
        Texture* texture;
        glm::vec3 position;
        glm::vec3 scale;
        ShaderProgram* shaderProgram;
        Renderer* renderer;
        float rotation;
        std::vector<float> vertexData;
        unsigned int indices[6] = {
            0, 1, 3,
            1, 2, 3
        };
        Sprite2D(std::string filePath, GLenum sampleMode = GL_LINEAR);
        void Render(double deltaTime);
        void setZForOrdering(float z) {
            position.z = z;
        }
    };
    class Camera : public System {
        public:
        glm::vec3 position;
        float zoom;
        ShaderProgram* shaderProgram;

        Camera(ShaderProgram* shaderProgram);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        void setPosition(glm::vec3 newPosition);
        void setZoom(float newZoom);
    };

    // Convert screen-space (top-left origin) to world-space (center origin, Y up) for the active camera
    inline glm::vec2 screenToWorld(const glm::vec2& screenPos) {
        Scene* scene = Engine::Engine::Instance().getCurrentScene();
        Camera* camera = scene ? scene->getCamera() : nullptr;
        if (!camera) return glm::vec2(0.0f);

        const float width = static_cast<float>(WindowStartupConfig::width);
        const float height = static_cast<float>(WindowStartupConfig::height);
        const float halfWidth = (width * 0.5f) / camera->zoom;
        const float halfHeight = (height * 0.5f) / camera->zoom;

        // Screen to normalized device coords (-1..1), flipping Y
        const float xNdc = (screenPos.x / width) * 2.0f - 1.0f;
        const float yNdc = 1.0f - (screenPos.y / height) * 2.0f;

        return glm::vec2(
            xNdc * halfWidth + camera->position.x,
            yNdc * halfHeight + camera->position.y
        );
    }
    class PhisicsSprite2D : public PhysicsObject2D, public Sprite2D {
    public:
        PhisicsSprite2D(std::string filePath, GLenum sampleMode) : PhysicsObject2D(0,0,glm::vec3(0.0f)), Sprite2D(filePath,sampleMode) {
            width = texture->width;
            height = texture->height;
            REGISTER_UPDATE(Update);
        }
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

 #endif //NIELS3DGAMEENGINE_RENDERER_H
