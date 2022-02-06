//
// Created by boris on 2/5/2022.
//

#ifndef VENGINE3D_VE_VECTOR4_H
#define VENGINE3D_VE_VECTOR4_H

namespace VE {
    struct Vector4 {
        void print() const {
            std::cout << x << " " << y << " " << z << " " << w << std::endl;
        }
        union {
            struct {
                float x;
                float y;
                float z;
                float w;
            };
            float v[4];
        };
    };
}


#endif //VENGINE3D_VE_VECTOR4_H
