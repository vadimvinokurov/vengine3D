//
// Created by boris on 11/16/2021.
//

#include "ve_world.h"
#include "Math/ve_matrix33.hpp"

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
    hid_CameraControl();
    hid_PositionControl();
}

void World::hid_CameraControl() {
    if (mouse_->isPressed(VE_MOUSE_BUTTON_3)) {
        mouse_->lockMouse();
        mouse_->setLockPosition();
    }
    if (mouse_->isRelease(VE_MOUSE_BUTTON_3)) {
        mouse_->unlockMouse();
    }
    if (mouse_->isRepeat(VE_MOUSE_BUTTON_3)) {
        currentCamera_->setDeltaRotation(
                VE::Vector(mouse_->deltaPosition().y(), 0, mouse_->deltaPosition().x() * -1) * mouseSense);
    }
}

void World::hid_PositionControl() {
    float speadUpDown = 0;
    if (keyboard_->isRepeat(VE_KEY_W)) {
        speadUpDown = 0.1;
    }
    if (keyboard_->isRepeat(VE_KEY_S)) {
        speadUpDown = -0.1;
    }
    float speadLeftRight = 0;
    if (keyboard_->isRepeat(VE_KEY_D)) {
        speadLeftRight = 0.1;
    }
    if (keyboard_->isRepeat(VE_KEY_A)) {
        speadLeftRight = -0.1;
    }

    VE::Vector normalUpDown(0, 0, -1);
    VE::Vector normalLeftRight(1, 0, 0);
    float alfa = currentCamera_->rotation().x() * M_PI / 180.0f;
    float vx[3][3] = {{1, 0,          0},
                      {0, cosf(alfa), -sinf(alfa)},
                      {0, sinf(alfa), cosf(alfa)}};

    alfa = currentCamera_->rotation().z() * M_PI / 180.0f;
    float vz[3][3] = {{cosf(alfa), -sinf(alfa), 0},
                      {sinf(alfa), cosf(alfa),  0},
                      {0,          0,           1}};

    VE::Matrix33 rx(vx);
    VE::Matrix33 rz(vz);

    normalUpDown = rx * normalUpDown;
    normalUpDown = rz * normalUpDown;
    currentCamera_->setPosition(currentCamera_->position() + normalUpDown * speadUpDown);

    normalLeftRight = rx * normalLeftRight;
    normalLeftRight = rz * normalLeftRight;
    currentCamera_->setPosition(currentCamera_->position() + normalLeftRight * speadLeftRight);
}


void VE::World::update(float dt) {
    hid();
}



