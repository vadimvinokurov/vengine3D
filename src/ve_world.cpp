//
// Created by boris on 11/16/2021.
//

#include "ve_world.h"

using namespace VE;

void VE::World::update(float dt) {
}

VE::World::World() {

}

const Camera &World::currentCamera() {
    return *currentCamera_;
}
