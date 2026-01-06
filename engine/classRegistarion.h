//
// Created by niel on 1/6/26.
//

#ifndef NIELS3DGAMEENGINE_CLASSREGISTARION_H
#define NIELS3DGAMEENGINE_CLASSREGISTARION_H

#include <unordered_map>
#include <string>
#include <functional>

class System {
public:
    virtual void Update(double delta){};
    virtual ~System() = default;
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
        return factories[name]();
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

#endif //NIELS3DGAMEENGINE_CLASSREGISTARION_H