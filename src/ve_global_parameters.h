//
// Created by boris on 12/2/2021.
//

#ifndef VENGINE3D_VE_GLOBAL_PARAMETERS_H
#define VENGINE3D_VE_GLOBAL_PARAMETERS_H

#include "math/ve_vector.h"

namespace VE{
    struct GlobalParameters{
        static Vector3 ligthPosition;
        static bool polygone;
        static bool warmstarting;
        static int iterations;
    };
}

extern VE::GlobalParameters globalParameters;


#endif //VENGINE3D_VE_GLOBAL_PARAMETERS_H
