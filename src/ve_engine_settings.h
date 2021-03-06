//
// Created by boris on 2/11/2022.
//

#ifndef VENGINE3D_VE_ENGINE_SETTINGS_H
#define VENGINE3D_VE_ENGINE_SETTINGS_H

namespace VE{
    struct VEngineSettings{
        static constexpr float VECTOR_EPSILON = 0.000001f;
        static constexpr float MATRIX_EPSILON = 0.000001f;
        static constexpr float TRANSFORM_EPSILON = 0.000001f;
        static constexpr float QUATERNION_EPSILON = 0.000001f;
        static constexpr int INFINITE_LOOP_INTERUPT = 10000;
    };
}

#endif //VENGINE3D_VE_ENGINE_SETTINGS_H
