//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_RENDER_H
#define VENGINE3D_RENDER_H

#include "ve_world.h"

namespace VE {
    class Render {
    public:
        explicit Render(float windowAspectRatio);
        void draw(const WorldPtr &world);
    private:
        void moveCamera();
        const float windowAspectRatio_ = 1.0f;
        WorldPtr world_;
    };
}


#endif //VENGINE3D_RENDER_H
