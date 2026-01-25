//
// Created by niel on 1/7/26.
//

#ifndef NIELS3DGAMEENGINE_RENDERER_H
#define NIELS3DGAMEENGINE_RENDERER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "glm-1.0.3/glm/glm.hpp"
#include  "glm-1.0.3/glm/gtc/matrix_transform.hpp"
#include "glm-1.0.3/glm/gtc/type_ptr.hpp"
#include "classRegistarion.h"
#include "engine.h"
#include "window.h"

namespace Engine {
    // Forward declarations
    class Texture;
    class PhysicsObject2D;
    class Engine;
    class Scene;

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
        Texture(std::string filePath) : Texture(filePath, Engine::Engine::Instance().defaultShaderProgram) {};
        Texture(std::string filePath, GLenum sampleMode) : Texture(filePath, Engine::Instance().defaultShaderProgram, sampleMode) {};
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
        Sprite2D(GLenum sampleMode = GL_LINEAR);
        void setTexture(Texture* tex);
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
    glm::vec2 screenToWorld(const glm::vec2& screenPos);
}

// Include these at the end to avoid circular dependencies
#include "engine.h"

#endif //NIELS3DGAMEENGINE_RENDERER_H
