//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_RENDER_H
#define VENGINE3D_RENDER_H

#include "ve_world.h"
#include "objects/ve_box_collider.h"
#include "objects/ve_sphere_collider.h"
#include "ve_shader.h"

namespace VE {
    class Render {
    public:
        explicit Render(float windowAspectRatio);
        void draw(const WorldPtr &world);
    private:
        Shader shader;
        const float windowAspectRatio_ = 1.0f;
        WorldPtr world_;
    };
}


#endif //VENGINE3D_RENDER_H
