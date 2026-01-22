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
        Texture(std::string filePath, ShaderProgram* shaderProgram);
        ~Texture();
    };
    inline std::vector<float> generateTextureVertices(float width, float height) {
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        
        return {
            // posities              // texture coords
             halfWidth,  halfHeight, 0.0f,  1.0f, 1.0f,   // rechtsboven
             halfWidth, -halfHeight, 0.0f,  1.0f, 0.0f,   // rechtsonder
            -halfWidth, -halfHeight, 0.0f,  0.0f, 0.0f,   // linksonder
            -halfWidth,  halfHeight, 0.0f,  0.0f, 1.0f    // linksboven
        };
    }
    
    inline std::vector<unsigned int> generateTextureIndices() {
        return {
            0, 1, 3,   // eerste driehoek
            1, 2, 3    // tweede driehoek
        };
    }
    
    class Sprite2D : public System {
        public:
        Texture* texture;
        glm::vec3 position;
        glm::vec3 scale;
        ShaderProgram* shaderProgram;
        Renderer* renderer;
        float rotation;
        float* vertexData;
        unsigned int indices[6] = {
            0, 1, 3,
            1, 2, 3
        };
        Sprite2D(std::string filePath);
        void Render(double deltaTime);
    };
}

#endif //NIELS3DGAMEENGINE_RENDERER_H
