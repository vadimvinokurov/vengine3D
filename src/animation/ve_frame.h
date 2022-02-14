//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_FRAME_H
#define VENGINE3D_VE_FRAME_H

namespace VE{
    template<typename T>
    struct Frame {
        static constexpr unsigned int N = sizeof(T) / sizeof(float);
        float value[N];
        float in[N];
        float out[N];
        float time;
    };
}



#endif //VENGINE3D_VE_FRAME_H
