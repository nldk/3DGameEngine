//
// Created by niel on 1/6/26.
//

#ifndef NIELS3DGAMEENGINE_CLASSREGISTARION_H
#define NIELS3DGAMEENGINE_CLASSREGISTARION_H

#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>
#include <stdexcept>

class System {
public:
    using UpdateFn = std::function<void(double)>;

    virtual ~System() = default;

    void RunUpdates(double delta) {
        for (auto& fn : updateVec) {
            fn(delta);
        }
    }

protected:
    std::vector<UpdateFn> updateVec;
};

class ClassRegistry {
public:
    using FactoryFn = std::function<System*()>;

    static ClassRegistry& Instance() {
        static ClassRegistry instance;
        return instance;
    }

    void Register(const std::string& name, FactoryFn fn) {
        factories[name] = fn;
    }

    System* Create(const std::string& name) {
        try {
            return factories[name]();
        }catch (const std::exception& e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
            std::exit(500);
        }catch (...) {
            std::cerr << "Unknown exception occurred" << std::endl;
            std::exit(400);
        }

    }

private:
    std::unordered_map<std::string, FactoryFn> factories;
};

struct ClassRegistrar {
    ClassRegistrar(const char* name, ClassRegistry::FactoryFn fn) {
        ClassRegistry::Instance().Register(name, fn);
    }
};

#define REGISTER_CLASS(TYPE)                                  \
static ClassRegistrar _registrar_##TYPE(                  \
#TYPE,                                                \
[]() -> System* { return new TYPE(); }                \
)

#define REGISTER_UPDATE(FN)\
updateVec.push_back(\
[this](double dt) { this->FN(dt); }\
)
#endif //NIELS3DGAMEENGINE_CLASSREGISTARION_H