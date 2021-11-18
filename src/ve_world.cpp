//
// Created by boris on 11/16/2021.
//

#include "ve_world.h"

using namespace VE;

World::World() {

}

const Camera &World::currentCamera() {
    return *currentCamera_;
}

void World::setHid(const KeyboardPtr &keyboard, const MousePtr &mouse) {
    keyboard_ = keyboard;
    mouse_ = mouse;
}

void World::hid() {
    float spead = 0;
    if (keyboard_->isRepeat(VE_KEY_W)) {
        spead = 0.1;
    }
    if (keyboard_->isRepeat(VE_KEY_S)) {
        spead = -0.1;
    }
    if (keyboard_->isRepeat(VE_KEY_A)) {

    }
    if (keyboard_->isRepeat(VE_KEY_D)) {

    }
    currentCamera_->setPosition(currentCamera_->position() + currentCamera_->rotation().normolize() * spead);
}

void VE::World::update(float dt) {
    hid();
}
