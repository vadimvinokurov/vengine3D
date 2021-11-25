//
// Created by boris on 11/16/2021.
//

#include "ve_world.h"
#include "Math/ve_matrix33.h"
#include "Object/ve_box_shape.h"

using namespace VE;

World::World() {

    VE::ShapePtr shape = std::make_shared<VE::BoxShape>();
    VE::RigidBodyPtr body1 = std::make_shared<VE::RigidBody>();
    body1->addCollider(shape);
    worldObjects.push_back(body1);


    VE::RigidBodyPtr body2 = std::make_shared<VE::RigidBody>();
    body2->addCollider(shape);
    worldObjects.push_back(body2);
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
    static int selectObject = 0;
    if (keyboard_->isPressed(VE_KEY_1)) {
        selectObject = (selectObject + 1) % 2;
    }

    if (keyboard_->isRepeat(VE_KEY_UP)) {
        if (keyboard_->isRepeat(VE_KEY_LEFT_SHIFT)) {
            worldObjects[selectObject]->moveTo(VE::Vector(0, 0, 0.05));
        } else {
            worldObjects[selectObject]->moveTo(VE::Vector(0, 0.05, 0));
        }
    }
    if (keyboard_->isRepeat(VE_KEY_DOWN)) {
        if (keyboard_->isRepeat(VE_KEY_LEFT_SHIFT)) {
            worldObjects[selectObject]->moveTo(VE::Vector(0, 0, -0.05));
        } else {
            worldObjects[selectObject]->moveTo(VE::Vector(0, -0.05, 0));
        }
    }
    if (keyboard_->isRepeat(VE_KEY_LEFT)) {
        worldObjects[selectObject]->moveTo(VE::Vector(-0.05, 0, 0));
    }
    if (keyboard_->isRepeat(VE_KEY_RIGHT)) {
        worldObjects[selectObject]->moveTo(VE::Vector(0.05, 0, 0));
    }

}

void World::hid_CameraControl() {
    if (mouse_->isPressed(VE_MOUSE_BUTTON_3)) {
        mouse_->lockMouse();
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
    float spead = 0.2f;
    float speadUpDown = 0;
    if (keyboard_->isRepeat(VE_KEY_W)) {
        speadUpDown = spead;
    }
    if (keyboard_->isRepeat(VE_KEY_S)) {
        speadUpDown = -spead;
    }
    float speadLeftRight = 0;
    if (keyboard_->isRepeat(VE_KEY_D)) {
        speadLeftRight = spead;
    }
    if (keyboard_->isRepeat(VE_KEY_A)) {
        speadLeftRight = -spead;
    }

    VE::Vector normalUpDown(0, 0, -1);
    VE::Vector normalLeftRight(1, 0, 0);

    float alfa = currentCamera_->rotation().x() * static_cast<float>(M_PI) / 180.0f;
    VE::Matrix33 rx(1, 0, 0,
                    0, cosf(alfa), -sinf(alfa),
                    0, sinf(alfa), cosf(alfa));

    alfa = currentCamera_->rotation().z() * static_cast<float>(M_PI) / 180.0f;
    VE::Matrix33 rz(cosf(alfa), -sinf(alfa), 0,
                    sinf(alfa), cosf(alfa), 0,
                    0, 0, 1);


    normalUpDown = rz * rx * normalUpDown;
    currentCamera_->setPosition(currentCamera_->position() + normalUpDown * speadUpDown);

    normalLeftRight = rz * rx * normalLeftRight;
    currentCamera_->setPosition(currentCamera_->position() + normalLeftRight * speadLeftRight);
}


void VE::World::update(float dt) {
    hid();
}



