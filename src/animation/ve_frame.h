//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_FRAME_H
#define VENGINE3D_VE_FRAME_H

namespace VE{
    template<unsigned int N>
    struct Frame {
        float value[N];
        float in[N];
        float out[N];
        float time;
    };

    using ScalarFrame = Frame<1>;
    using VectorFrame = Frame<3>;
    using QuaternionFrame = Frame<4>;
}



#endif //VENGINE3D_VE_FRAME_H
