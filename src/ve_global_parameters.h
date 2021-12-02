//
// Created by boris on 12/2/2021.
//

#ifndef VENGINE3D_VE_GLOBAL_PARAMETERS_H
#define VENGINE3D_VE_GLOBAL_PARAMETERS_H

#include "Math/ve_vector.h"

namespace VE{
    struct GlobalParameters{
        int epaIterations = 1;
        int polytopeStage = 0;
        float cameraSpeed = 0.05;
        VE::Vector minEpaNormal;
        int pointSize = 0;
        std::vector<float> direction;
        float rotate[4];

    };
}

extern VE::GlobalParameters globalParameters;


#endif //VENGINE3D_VE_GLOBAL_PARAMETERS_H
