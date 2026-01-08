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

        // utility uniform functions
        // ------------------------------------------------------------------------
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

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
    };

    class Renderer {
        public:
        ShaderProgram* shaderProgram;
        Renderer(ShaderProgram* shaderProgram);
        void loadData(float *vertex, int vertexCount, unsigned int* indices, int indexCount);
        void setVAtributes(int layout, unsigned int size, GLenum type, GLboolean normalize, unsigned int stride, unsigned int offset);
        void render(unsigned int start, unsigned int count,GLenum type,Texture* tex);
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
        Texture(std::string filePath);
        ~Texture();
    };
}

#endif //NIELS3DGAMEENGINE_RENDERER_H
