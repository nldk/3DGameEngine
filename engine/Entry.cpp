//
// Created by niel on 1/6/26.
//
#include <fstream>
#include <iostream>

#include "classRegistarion.h"
#include "engine.h"
#include "../glad/glad/glad.h"
#include "../glfw-3.4/include/GLFW/glfw3.h"
#include "../engine/window.h"
#include "../engine/utils.h"

#define WIDHT 500
#define HEIGHT 500



int main() {

    glfwInit();
    std::unordered_map<std::string, std::string> config;

    std::ifstream file(gamePath + "testGame.project");
    std::string line;


    while (std::getline(file, line)) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);

        config[key] = value; // O(1) average
    }
    std::string initFuncName;
    if (config.contains("initClass")) {
        initFuncName = config["initClass"];
    }else {
        std::cout << "initClass not found in config" << std::endl;
        throw std::runtime_error("initClass not found in config");
    }
    System* initClass = ClassRegistry::Instance().Create(initFuncName);
    if(!initClass) {
        std::cout << "init class not found" << std::endl;
        throw std::runtime_error("init class not found");
    }
    int winW = (WindowStartupConfig::width > 0) ? WindowStartupConfig::width : WIDHT;
    int winH = (WindowStartupConfig::height > 0) ? WindowStartupConfig::height : HEIGHT;
    const char* title = (WindowStartupConfig::title.empty() ? "3DGameEngine" : WindowStartupConfig::title.c_str());
    int maj = (WindowStartupConfig::gl_version_major > 0) ? WindowStartupConfig::gl_version_major : 3;
    int min = (WindowStartupConfig::gl_version_minor > 0) ? WindowStartupConfig::gl_version_minor : 3;
    Engine::Window* win = new Engine::Window(winW, winH, title, maj, min);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    win->setGLViewport(0,0,WIDHT,HEIGHT);
    win->background = Engine::Vec4(0.2f, 0.3f, 0.3f, 1.0f);
    Engine::Engine::Instance().window = win;
    std::string scenePath = gamePath;
    if (config.contains("startScene")) {
        scenePath += config["startScene"];
    } else {
        scenePath += "testScene.scene";
    }
    Engine::Engine::Initialize(scenePath);
    Engine::Engine::Instance().start();
    Engine::Engine::Instance().runUpdateLoop();
    glfwTerminate();
}