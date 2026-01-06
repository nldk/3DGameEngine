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
class TestClass : public System {
    public:
    TestClass() {
        std::cout << "testClass" << std::endl;
    }
    void Update(double deltaTime) {
        std::cout << "testClass " << deltaTime << std::endl;
    }
};
REGISTER_CLASS(TestClass);