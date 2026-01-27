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
#include <vector>

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
        auto it = factories.find(name);
        if (it != factories.end()) {
            std::cerr << "ClassRegistry warning: replacing factory for '" << name << "'" << std::endl;
        }
        factories[name] = fn;
        std::cout << "ClassRegistry::Register => " << name << " factory @" << reinterpret_cast<const void*>(&fn) << std::endl;
    }

    System* Create(const std::string& name) {
        auto it = factories.find(name);
        if (it == factories.end()) {
            std::cerr << "ClassRegistry error: factory not found for '" << name << "'" << std::endl;
            return nullptr;
        }
        try {
            System* created = it->second();
            std::cout << "ClassRegistry::Create => " << name << " instance @" << created << std::endl;
            return created;
        } catch (const std::exception& e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
            return nullptr;
        } catch (...) {
            std::cerr << "Unknown exception occurred" << std::endl;
            return nullptr;
        }

    }
    const std::unordered_map<std::string, FactoryFn>& getRegisteredFactories() const {
        return factories;
    };
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
namespace Engine {
    inline std::vector<System*> getRegisteredClasses(const std::string& name) {
        std::vector<System*> result;
        for (const auto & pair : ClassRegistry::Instance().getRegisteredFactories()) {
            if (pair.first == name) {
                System* instance = pair.second();
                result.push_back(instance);
            }
        }
        return result;
    }
}

#endif //NIELS3DGAMEENGINE_CLASSREGISTARION_H