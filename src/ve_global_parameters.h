//
// Created by boris on 12/2/2021.
//

#ifndef VENGINE3D_VE_GLOBAL_PARAMETERS_H
#define VENGINE3D_VE_GLOBAL_PARAMETERS_H

#include "Math/ve_vector.h"

namespace VE{
    struct GlobalParameters{
        Vector rotate;
        bool polygone = true;
        bool warmstarting = true;
        int iterations = 10;
    };
}

extern VE::GlobalParameters globalParameters;


#endif //VENGINE3D_VE_GLOBAL_PARAMETERS_H
