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
#include "phisycs.h"

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
        void setVec4(const std::string &name, const glm::vec4 &value) const {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
        }
        void setVec2(const std::string &name, const glm::vec2 &value) const {
             glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
         }
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
        unsigned int VBO{0};
        unsigned int VAO{0};
        unsigned int EBO{0};
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

    inline std::vector<float> generateTextureVertices(float width, float height, const glm::vec2& uvMin, const glm::vec2& uvMax) {
         // Normalize quad around origin; UVs are supplied so callers can crop sprite sheets
         // Inverted V (Y texture coordinate) to fix vertical mirroring in OpenGL
         return {
             // positions            // texture coords
              0.5f,  0.5f, 0.0f,  uvMax.x, uvMin.y,
              0.5f, -0.5f, 0.0f,  uvMax.x, uvMax.y,
             -0.5f, -0.5f, 0.0f,  uvMin.x, uvMax.y,
             -0.5f,  0.5f, 0.0f,  uvMin.x, uvMin.y
         };
     }

     inline std::vector<float> generateTextureVertices(float width, float height) {
        // Normalize to 1x1 units regardless of texture dimensions
        // This allows scale to control actual size in pixels
        // Inverted V (Y texture coordinate) to fix vertical mirroring in OpenGL
        return {
            // posities              // texture coords
             0.5f,  0.5f, 0.0f,  1.0f, 0.0f,   // rechtsboven
             0.5f, -0.5f, 0.0f,  1.0f, 1.0f,   // rechtsonder
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f,   // linksonder
            -0.5f,  0.5f, 0.0f,  0.0f, 0.0f    // linksboven
        };
    }

    inline std::vector<unsigned int> generateTextureIndices() {
        return {
            0, 1, 3,   // eerste driehoek
            1, 2, 3    // tweede driehoek
        };
    }

    class DebugRenderer2D {
    public:
        explicit DebugRenderer2D(ShaderProgram* shaderProgram);
        ~DebugRenderer2D();
        void drawHitboxes(const std::vector<WorldHitbox2D>& boxes, const Camera& camera, const glm::vec4& color);
    private:
        ShaderProgram* shaderProgram;
        unsigned int VAO{0};
        unsigned int VBO{0};
        unsigned int EBO{0};
        void ensureBuffers();
    };

    class Sprite2D : virtual public System {
        public:
        Texture* texture;
        glm::vec3 position;
        glm::vec3 scale;
        bool flipX{false};
        bool flipY{false};
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
        void setFlip(bool flipX, bool flipY) {
            this->flipX = flipX;
            this->flipY = flipY;
        }
        void Render(double deltaTime);
        void setZForOrdering(float z) {
            position.z = z;
        }
    };

    class PhysicsSprite2D : public PhysicsObject2D, public Sprite2D {
    public:
        PhysicsSprite2D(std::string filePath, GLenum sampleMode) : PhysicsObject2D(0,0,glm::vec3(0.0f)), Sprite2D(filePath,sampleMode) {
            width = texture ? texture->width : 0;
            height = texture ? texture->height : 0;
            hitboxes.clear();
            if (width > 0 && height > 0) {
                addHitbox(width, height);
            }
            REGISTER_UPDATE(UpdateSP);
        }
        PhysicsSprite2D(Texture* tex, GLenum sampleMode): PhysicsObject2D(0,0,glm::vec3(0.0f)), Sprite2D(sampleMode) {
            setTexture(tex);
            width = tex ? tex->width : 0;
            height = tex ? tex->height : 0;
            hitboxes.clear();
            if (width > 0 && height > 0) {
                addHitbox(width, height);
            }
            REGISTER_UPDATE(UpdateSP);
        }
        PhysicsSprite2D(GLenum sampleMode): PhysicsObject2D(0,0,glm::vec3(0.0f)), Sprite2D(sampleMode) {
            width = 0;
            height = 0;
            hitboxes.clear();
            REGISTER_UPDATE(UpdateSP);
        };
        void UpdateSP(double deltaTime) {
            Sprite2D::position = PhysicsObject2D::position;
        }
        void setPosition(glm::vec2 newPosition) {
            PhysicsObject2D::position = glm::vec3(newPosition, 0.0f);
            Sprite2D::position = glm::vec3(newPosition, 0.0f);
        }
        void setScale(glm::vec3 newScale) {
            Sprite2D::scale = newScale;
        }
        void setColliderSize(const glm::vec2& size) {
            setSingleColliderSize(size);
        }
        glm::vec2 getColliderSize() const {
            return glm::vec2(static_cast<float>(width), static_cast<float>(height));
        }
        glm::vec2 getPositionOfP() {
            return glm::vec2(PhysicsObject2D::position.x, PhysicsObject2D::position.y);
        }
        glm::vec2 getPositionOfS() {
            return glm::vec2(Sprite2D::position.x, Sprite2D::position.y);
        }
    };
    using PhisicsSprite2D = PhysicsSprite2D;

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
    class Animation2D {
    public:
        float frameDuration; // Duration of each frame in seconds
        std::vector<Texture*> frames;
        Animation2D(float frameDuration) : frameDuration(frameDuration) {}
        void addFrame(Texture* frame) {
            frames.push_back(frame);
        }
        void clearFrames() {
            frames.clear();
        }
        void nextFrame() {
            if (frames.empty()) return;
            currentFrameIndex = (currentFrameIndex + 1) % frames.size();
        }
        Texture* getCurrentFrame() const {
            if (frames.empty()) return nullptr;
            return frames[currentFrameIndex];
        }
        void Update(double deltaTime) {
            static double timeAccumulator = 0.0;
            timeAccumulator += deltaTime;
            if (timeAccumulator >= frameDuration) {
                nextFrame();
                timeAccumulator = 0.0;
            }
        }
        private:
        size_t currentFrameIndex = 0;
    };
    class AnimatedSprite2D : public Sprite2D {
    public:
        std::unordered_map<std::string, Animation2D*> animations;
        Animation2D* currentAnimation = nullptr;
        AnimatedSprite2D(GLenum sampleMode = GL_LINEAR) : Sprite2D(sampleMode) {
            REGISTER_UPDATE(UpdateAS);
            // Render is already registered by Sprite2D constructor
        }
        void addAnimation(const std::string& name, Animation2D* animation) {
            animations[name] = animation;
        }
        void playAnimation(const std::string& name) {
            auto it = animations.find(name);
            if (it != animations.end()) {
                currentAnimation = it->second;
            }
        }
        void UpdateAS(double deltaTime) {
            if (currentAnimation) {
                currentAnimation->Update(deltaTime);
                Texture* frame = currentAnimation->getCurrentFrame();
                if (frame) {
                    setTexture(frame);
                }
            }
        }
    };
    class AnimatedPyshicsSprite2D : public AnimatedSprite2D, public PhysicsObject2D {
    public:
        AnimatedPyshicsSprite2D(GLenum sampleMode = GL_LINEAR)
            : PhysicsObject2D(0, 0, glm::vec3(0.0f)), AnimatedSprite2D(sampleMode) {
            REGISTER_UPDATE(UpdateAPS);
        }
        void UpdateAPS(double deltaTime) {
            AnimatedSprite2D::position = PhysicsObject2D::position;
        }
        void setPosition(glm::vec2 newPosition) {
            PhysicsObject2D::position = glm::vec3(newPosition, 0.0f);
            AnimatedSprite2D::position = glm::vec3(newPosition, 0.0f);
        }
        void setScale(glm::vec3 newScale) {
            AnimatedSprite2D::scale = newScale;
        }
        void setColliderSize(const glm::vec2& size) {
            setSingleColliderSize(size);
        }
        glm::vec2 getColliderSize() const {
            return glm::vec2(static_cast<float>(width), static_cast<float>(height));
        }
        glm::vec2 getPositionOfP() {
            return glm::vec2(PhysicsObject2D::position.x, PhysicsObject2D::position.y);
        }
        glm::vec2 getPositionOfS() {
            return glm::vec2(Sprite2D::position.x, Sprite2D::position.y);
        }
    };
 }

// Include these at the end to avoid circular dependencies
#include "engine.h"

#endif //NIELS3DGAMEENGINE_RENDERER_H
