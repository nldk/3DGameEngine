//
// Created by niel on 1/5/26.
//

#ifndef NIELS3DGAMEENGINE_WINDOW_H
#define NIELS3DGAMEENGINE_WINDOW_H
#include <functional>
#include <string>

#include "../glad/glad.h"
#include "../glfw-3.4/include/GLFW/glfw3.h"
#include "utils.h"
#include "glm-1.0.3/glm/vec2.hpp"
#include "glm-1.0.3/glm/vec4.hpp"
namespace Engine {
    class Window {
        public:
        glm::vec4 background = glm::vec4(0.0f,0.0f,0.0f,0.0f);
        GLFWwindow* window;
        Window(int width, int height, const char* title,int gl_version_major, int gl_Version_minor);
        void update();
        void destroy();
        void setGLViewport(int x, int y, int width, int height);
        virtual void resize(int x,int y, int width,int height);
        bool isKeyPressed(int key);
        bool isKeyReleased(int key);
        bool isMouseButtonPressed(int key);
        glm::vec2 getMousePosition() {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
        };
        void clearGLCollorBit();
        int getViewportX() const {;
            return viewportX;
        };
        int getViewportY() const {
            return viewportY;
        };
        glm::vec2 getMouseScrollOffset() {
            return glm::vec2(scrollxoffset, scrollyoffset);
        }
    private:
        float scrollxoffset,scrollyoffset;
        int viewportX,viewportY;
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
            auto* self = static_cast<Engine::Window*>(glfwGetWindowUserPointer(window));
            if (!self) return;
            self->resize(self->viewportX,self->viewportY , width, height);
        };
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
            auto* self = static_cast<Engine::Window*>(glfwGetWindowUserPointer(window));
            if (!self) return;
            self->scrollxoffset = static_cast<int>(xoffset);
            self->scrollyoffset = static_cast<int>(yoffset);
        };
    };
}

class WindowStartupConfig {
    public:
    static int width,height;
    static std::string title;
    static int gl_version_major,gl_version_minor;
    static bool vSync;
    WindowStartupConfig(int width,int height,std::string title,int gl_version_major,int gl_version_minor,bool vSync) {;
        WindowStartupConfig::width = width;
        WindowStartupConfig::height = height;
        WindowStartupConfig::title = title;
        WindowStartupConfig::gl_version_major = gl_version_major;
        WindowStartupConfig::gl_version_minor = gl_version_minor;
        WindowStartupConfig::vSync = vSync;
    };
};
#endif //NIELS3DGAMEENGINE_WINDOW_H