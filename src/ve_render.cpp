//
// Created by boris on 11/16/2021.
//

#include "ve_render.h"
#include <iostream>

#include <GL/gl.h>

using namespace VE;

Render::Render(const WorldPtr &world) : world_(world) {}

void Render::draw() {
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
