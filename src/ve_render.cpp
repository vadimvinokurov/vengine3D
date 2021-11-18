//
// Created by boris on 11/16/2021.
//

#include "ve_render.h"
#include "Math/ve_vector.hpp"

#include <iostream>

#include <GL/gl.h>


using namespace VE;

Render::Render(float windowAspectRatio) : windowAspectRatio_(windowAspectRatio) {
    glOrtho(-windowAspectRatio_, windowAspectRatio_, -1, 1, -1, 1);
}

void Render::draw(const World &world) {
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    VE::Vector a(0.5, 0.5, 0);
    a.draw();
}
