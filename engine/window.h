//
// Created by niel on 1/5/26.
//

#ifndef NIELS3DGAMEENGINE_WINDOW_H
#define NIELS3DGAMEENGINE_WINDOW_H
#include <functional>
#include <string>

#include "../glad/glad/glad.h"
#include "../glfw-3.4/include/GLFW/glfw3.h"
#include "utils.h"
namespace Engine {
    class Window {
        public:
        Vec4 background = Vec4(0.0f,0.0f,0.0f,0.0f);
        GLFWwindow* window;
        Window(int width, int height, const char* title,int gl_version_major, int gl_Version_minor);
        void update();
        void destroy();
        void setGLViewport(int x, int y, int width, int height);
        virtual void resize(int x,int y, int width,int height);
        bool isKeyPressed(int key);
        bool isKeyReleased(int key);
        void clearGLCollorBit();
    private:
        int viewportX,viewportY;
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        auto* self = static_cast<Engine::Window*>(glfwGetWindowUserPointer(window));
        if (!self) return;
        self->resize(self->viewportX,self->viewportY , width, height);
        };
    };
}

class WindowStartupConfig {
    public:
    static int width,height;
    static std::string title;
    static int gl_version_major,gl_version_minor;
    WindowStartupConfig(int width,int height,std::string title);
};
#endif //NIELS3DGAMEENGINE_WINDOW_H