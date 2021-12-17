//
// Created by boris on 11/16/2021.
//

#include "ve_world.h"
#include "objects/ve_box_collider.h"
#include "objects/ve_sphere_collider.h"
#include "imgui/imgui.h"
#include "ve_global_parameters.h"

using namespace VE;

World::World() {
    resetScene();
}

void World::resetScene() {
    worldObjects.clear();
    contactSolvers.clear();

    for(int i = 0; i < 10; i++){
        auto body1 = std::make_shared<VE::RigidBody>();
        auto collider1 = std::make_shared<VE::BoxCollider>();
        body1->addCollider(collider1);
        body1->setTransform([i]() {
            Transform transform;
            transform.position = Vector(0.5f, 0.5f, 0.6f + 1.2f * static_cast<float>(i));
            return transform;
        }());
        body1->setGravity(Vector(0.0f, 0.0f, -9.8f));
        worldObjects.push_back(body1);
    }

    auto body1 = std::make_shared<VE::RigidBody>();
    auto collider1 = std::make_shared<VE::BoxCollider>();
    body1->addCollider(collider1);
    body1->setTransform([]() {
        Transform transform;
        transform.position = Vector(5.5f, 0.5f, 0.6f + 10.2f);
        return transform;
    }());
    body1->setRestitution(0.99f);
    body1->setGravity(Vector(0.0f, 0.0f, -9.8f));
    worldObjects.push_back(body1);



//    auto body2 = std::make_shared<VE::RigidBody>();
//    body2->addCollider(std::make_shared<VE::BoxCollider>(1,1,1,10));
//    body2->setGravity(Vector(0.0f, 0.0f, -9.8f));
//    body2->setTransform([]() {
//        Transform transform;
//        transform.position = Vector(0, 0, 2);
//        return transform;
//    }());
//    worldObjects.push_back(body2);

    auto floor = std::make_shared<VE::RigidBody>();
    auto floarCol = std::make_shared<VE::BoxCollider>(100, 1, 100, 0);
    floarCol->setColor(Color(0.3f, 0.3f, 0.3f));
    floor->addCollider(floarCol);
    floor->setTransform([]() {
        Transform transform;
        transform.position = Vector(0, 0, -0.5f);
        return transform;
    }());
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
    if (worldObjects.size() == 0) return;

    for (size_t i = 0; i < worldObjects.size() - 1; i++) {
        for (size_t j = i + 1; j < worldObjects.size(); j++) {
            VE::RigidBody &body1 = *worldObjects[i];
            VE::RigidBody &body2 = *worldObjects[j];
            if (body1.invMass() + body2.invMass() == 0) continue;
            //if (!(body1.isCollision() && body2.isCollision())) continue;

            VE::ContactKey contactKey(body1, body2);
//            if (!aabbCollisionDetection(body1, body2)) {
//                contactSolvers.erase(contactKey);
//                continue;
//            }

            VE::ContactSolver newContact(body1, body2);
            if (newContact.isCollide()) {
                auto citer = contactSolvers.find(contactKey);
                if (citer == contactSolvers.end()) {
                    contactSolvers.emplace(contactKey, newContact);
                } else {
                    citer->second.update(newContact.contactMainfold());
                }
            } else {
                contactSolvers.erase(contactKey);
            }
        }
    }
}

void World::physics(float dt) {
    for (auto &object: worldObjects) {
        object->updateVelocity(dt);
    }

    for (auto &contact: contactSolvers) {
        contact.second.preStep(dt);
    }

    for (int i = 0; i < globalParameters.iterations; i++) {
        for (auto &contact: contactSolvers) {
            contact.second.applyImpulse(dt);
        }
    }

    for (int i = 0; i < globalParameters.iterations; i++) {
        for (auto &contact: contactSolvers) {
            contact.second.applyPseudoImpulse(dt);
        }
    }

    for (auto &object: worldObjects) {
        object->updateTransform(dt);
    }
}

void World::gui() {
    ImGui::Begin("Control panel");
    if (ImGui::Button("Reset")) resetScene();
    ImGui::SliderFloat("Camera speed", &globalParameters.cameraSpeed, 0.05f / 20, 0.05f * 4);
    ImGui::SliderInt("iteration", &globalParameters.iterations, 1, 200);
    ImGui::Checkbox("Warnstarting", &globalParameters.warmstarting);
    ImGui::Checkbox("pseudoVelosity", &globalParameters.pseudoVelosity);

    ImGui::End();

}







