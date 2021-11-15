//
// Created by boris on 11/16/2021.
//

#include "Render.h"
#include <iostream>
using namespace VE;
Render::Render(const WorldPtr &world) : world_(world) {
    std::cout << "Print()" << std::endl;
}

void Render::operator()() {
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
