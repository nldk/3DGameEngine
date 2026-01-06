//
// Created by niel on 1/5/26.
//

#ifndef NIELS3DGAMEENGINE_UTILS_H
#define NIELS3DGAMEENGINE_UTILS_H

namespace Engine{
    class Vec2 {
        public:
        float x,y;
        Vec2(float x,float y):x(x),y(y){};
    };
    class Vec3 {
        public:
        float x,y,z;
        Vec3(float x,float y,float z):x(x),y(y),z(z){};
    };
    class Vec4 {
        public:
        float x,y,z,w;
        Vec4(float x, float y, float z, float w):x(x),y(y),z(z),w(w){};
    };
}

#endif //NIELS3DGAMEENGINE_UTILS_H