//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_RENDER_H
#define VENGINE3D_RENDER_H

#include "GL/gl.h"
#include "World.h"

namespace VE {
    class Render {
    public:
        explicit Render(const WorldPtr &world);
        void operator()();
    private:

        const WorldPtr &world_;
    };
}


#endif //VENGINE3D_RENDER_H
