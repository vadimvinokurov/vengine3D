//
// Created by boris on 11/16/2021.
//

#include "ve_world.h"
#include "math/ve_matrix33.h"
#include "objects/ve_box_collider.h"
#include "collision/ve_collision.h"
#include "physics/ve_contact_solver.h"
#include "imgui/imgui.h"
#include "ve_global_parameters.h"

using namespace VE;

World::World() {
    resetScene();
}

void World::resetScene() {
    worldObjects.clear();

    auto body1 = std::make_shared<VE::RigidBody>();
    auto collider1 = std::make_shared<VE::BoxCollider>();
    body1->addCollider(collider1);
    body1->setTransform([]() {
        Transform transform;
        transform.position = Vector(0.4f, 0.4f, 4);
        return transform;
    }());
    body1->setGravity(Vector(0.0f, 0.0f, -9.8f));
    worldObjects.push_back(body1);

    auto body2 = std::make_shared<VE::RigidBody>();
    body2->addCollider(std::make_shared<VE::BoxCollider>());
    body2->setGravity(Vector(0.0f, 0.0f, -9.8f));
    body2->setTransform([]() {
        Transform transform;
        transform.position = Vector(0, 0.1f, 2);
        return transform;
    }());
    worldObjects.push_back(body2);

    auto floor = std::make_shared<VE::RigidBody>();
    auto floarCol = std::make_shared<VE::BoxCollider>(100, 1, 100, 0);
    floarCol->setColor(Color(0.3f, 0.3f, 0.3f));
    floor->addCollider(floarCol);

    worldObjects.push_back(floor);
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
    if (keyboard_->isPressed(VE_KEY_F1)) {
        selectObject = (selectObject + 1) % 2;
    }

    float speed = globalParameters.cameraSpeed;
    if (keyboard_->isRepeat(VE_KEY_UP)) {
        if (keyboard_->isRepeat(VE_KEY_LEFT_SHIFT)) {
            worldObjects[selectObject]->moveTo(VE::Vector(0, 0, speed));
        } else {
            worldObjects[selectObject]->moveTo(VE::Vector(0, speed, 0));
        }
    }
    if (keyboard_->isRepeat(VE_KEY_DOWN)) {
        if (keyboard_->isRepeat(VE_KEY_LEFT_SHIFT)) {
            worldObjects[selectObject]->moveTo(VE::Vector(0, 0, -speed));
        } else {
            worldObjects[selectObject]->moveTo(VE::Vector(0, -speed, 0));
        }
    }
    if (keyboard_->isRepeat(VE_KEY_LEFT)) {
        worldObjects[selectObject]->moveTo(VE::Vector(-speed, 0, 0));
    }
    if (keyboard_->isRepeat(VE_KEY_RIGHT)) {
        worldObjects[selectObject]->moveTo(VE::Vector(speed, 0, 0));
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
    gui();
    hid();
    prephysics(dt);
    physics(dt);
}

void World::prephysics(float dt) {
}

void World::physics(float dt) {

    for (auto &object: worldObjects) {
        object->update(dt);
    }

    for (size_t i = 0; i < worldObjects.size() - 1; i++) {
        for (size_t j = i + 1; j < worldObjects.size(); j++) {
            VE::RigidBody &body1 = *worldObjects[i];
            VE::RigidBody &body2 = *worldObjects[j];
            VE::ContactMainfold contactMainfold;
            if (testIntersection(body1, body2, contactMainfold)) {
                for (auto &contact:contactMainfold) {
                    contact.point.drawPoint(12, Color(1, 0, 0));
                }
            };

            auto a = ContactSolver(body1, body2);
            a.preStep(dt);
            a.applyImpulse(dt);
        }
    }


}

void World::gui() {
    ImGui::Begin("Control panel");
    if (ImGui::Button("Reset")) resetScene();
    ImGui::SliderFloat("Camera speed", &globalParameters.cameraSpeed, 0.05f / 20, 0.05f * 4);
    ImGui::SliderInt("face", &globalParameters.clippingIteration, 0, 16);
    ImGui::SliderInt("vector", &globalParameters.verctorNumber, 0, 3);
    ImGui::ColorEdit4("Color", globalParameters.rotate);
    ImGui::Text("Polytope vertex - %d:", globalParameters.pointSize);
    //ImGui::Text("minimal epa normal - %f %f %f:", globalParameters.minEpaNormal.x(),globalParameters.minEpaNormal.y(),globalParameters.minEpaNormal.z());

    ImGui::BeginChild("Scrolling");
    for (float v: globalParameters.direction) {
        ImGui::Text("same direction %f", v);
    }

    ImGui::EndChild();

    ImGui::End();

}







