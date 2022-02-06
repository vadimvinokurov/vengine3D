//
// Created by boris on 2/5/2022.
//

#ifndef VENGINE3D_VE_VECTOR4_H
#define VENGINE3D_VE_VECTOR4_H

namespace VE {
    struct Vector4 {
        static constexpr float EPSILON = 0.000001f;

        bool operator==(const Vector4 &other) const {
            return (fabsf(this->x - other.x) < EPSILON) &&
                   (fabsf(this->y - other.y) < EPSILON) &&
                   (fabsf(this->z - other.z) < EPSILON) &&
                   (fabsf(this->w - other.w) < EPSILON);
        }

        bool operator!=(const Vector4 &other) const {
            return !(*this == other);
        }

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
