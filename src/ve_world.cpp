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


void World::scene1() {
    RigidBodyPtr floor = RigidBody::create({BoxCollider::create(100, 1, 100, 0)});
    floor->setTransform([]() {
        Transform transform;
        transform.position = Vector(0, 0, -0.5f);
        return transform;
    }());
    floor->setColor(Color(0.3f, 0.3f, 0.3f));
    worldObjects.push_back(floor);


    auto spawBox = [&](const Transform &transform) {
        RigidBodyPtr body1 = RigidBody::create({BoxCollider::create(1, 1, 1, 1)});
        body1->setTransform(transform);
        body1->setGravity(Vector(0.0f, 0.0f, -9.8f));
        worldObjects.push_back(body1);
    };

    for (int i = 0; i < 10; i++) {
        Transform transform;
        transform.position = Vector(0.5f, 0.5f, 0.6f + 1.2f * static_cast<float>(i));
        spawBox(transform);
    }
}

void World::scene2() {
    auto spawBox = [&](const Transform &transform) {
        auto body1 = RigidBody::create({BoxCollider::create()});
        body1->setTransform(transform);
        body1->setGravity(Vector(0.0f, 0.0f, -9.8f));
        worldObjects.push_back(body1);
        return body1;
    };

    Vector offset(10, 3, 0);

    Transform transform;
    transform.position = Vector(0.5f, 0.5f, 0.5f) + offset;
    auto o = spawBox(transform);

    jointSolver_ = std::make_shared<VE::MouseJointSolver>(o, Vector(0, 0, 1) + offset);
}

void World::scene3() {
    RigidBodyPtr floor = RigidBody::create({BoxCollider::create(100, 1, 100, 0)});
    floor->setTransform([]() {
        Transform transform;
        transform.position = Vector(0, 0, -0.5f);
        return transform;
    }());
    floor->setColor(Color(0.3f, 0.3f, 0.3f));
    worldObjects.push_back(floor);

    std::vector<VE::ColliderPtr> a;
    RigidBodyPtr body1 = RigidBody::create(
            {
                    BoxCollider::create(1, 1, 1, 1, Vector(0.0f, 0.0f, 0.0f)),
            });
    body1->setTransform([]() {
        Transform transform;
        transform.position = Vector(0, 0, 1.5f);
        return transform;
    }());
    body1->setGravity(Vector(0.0f, 0.0f, -9.8f));
    worldObjects.push_back(body1);


    RigidBodyPtr body2 = RigidBody::create(
            {
                    BoxCollider::create(1, 1, 1, 1, Vector(0.0f, 0.0f, 0.0f)),
                    BoxCollider::create(1, 1, 1, 1, Vector(1.0f, 0.0f, 0.0f))
            });
    body2->setTransform([]() {
        Transform transform;
        transform.position = Vector(0, 0.6f, 10.5f);
        return transform;
    }());
    body2->setGravity(Vector(0.0f, 0.0f, -9.8f));
    worldObjects.push_back(body2);

}

void World::scene4() {
    RigidBodyPtr floor = RigidBody::create({BoxCollider::create(100, 1, 100, 0)});
    floor->setTransform([]() {
        Transform transform;
        transform.position = Vector(0, 0, -0.5f);
        return transform;
    }());
    floor->setColor(Color(0.3f, 0.3f, 0.3f));
    worldObjects.push_back(floor);

    RigidBodyPtr body2 = RigidBody::create(
            {
                    SphereCollider::create(1, 1, Vector(0.0f, 0.0f, 0.0f)),
            });
//    body2->setTransform([]() {
//        Transform transform;
//        transform.position = Vector(0, 0.6f, 10.5f);
//        return transform;
//    }());
//    body2->setGravity(Vector(0.0f, 0.0f, -9.8f));
    worldObjects.push_back(body2);

}

void World::resetScene() {
    worldObjects.clear();
    contactSolvers.clear();

    scene4();
}

const Camera &World::currentCamera() {
    return *currentCamera_;
}

void World::setHid(const KeyboardPtr &keyboard, const MousePtr &mouse) {
    keyboard_ = keyboard;
    mouse_ = mouse;
}


void World::hid(float dt) {
    if (mouse_->isPressed(VE_MOUSE_BUTTON_1)) {
        mouseJointSolver_.reset();
        float minLen = std::numeric_limits<float>::max();
        RigidBodyPtr selectObject;
        for (auto &object:worldObjects) {
            float currentLen = (object->centerOfMass() - currentCamera_->getPointAlongDirection(10)).sqrtAbs();
            if (currentLen < minLen) {
                minLen = currentLen;
                selectObject = object;
            }
        }
        if (minLen != std::numeric_limits<float>::max()) {
            mouseJointSolver_ = std::make_shared<VE::MouseJointSolver>(selectObject, selectObject->centerOfMass());
        }
    }
    if (mouse_->isRelease(VE_MOUSE_BUTTON_1)) {
        mouseJointSolver_.reset();
    }
    if (keyboard_->isPressed(VE_KEY_F1)) {
        globalParameters.polygone = !globalParameters.polygone;
    }
    cameraControl(dt);
}

void World::cameraControl(float dt) {
    float cameraSpeed = 15.0f * dt;

    if (mouse_->isPressed(VE_MOUSE_BUTTON_2)) {
        mouse_->lockMouse();
    }
    if (mouse_->isRelease(VE_MOUSE_BUTTON_2)) {
        mouse_->unlockMouse();
    }
    if (mouse_->isRepeat(VE_MOUSE_BUTTON_2)) {
        currentCamera_->setDirection(mouse_->deltaPosition().y(), mouse_->deltaPosition().x() * -1);
    }

    if (keyboard_->isRepeat(VE_KEY_W)) {
        currentCamera_->moveAlongDirection(cameraSpeed);
    }
    if (keyboard_->isRepeat(VE_KEY_S)) {
        currentCamera_->moveAlongDirection(-cameraSpeed);
    }
    if (keyboard_->isRepeat(VE_KEY_D)) {
        currentCamera_->moveAlongSide(cameraSpeed);
    }
    if (keyboard_->isRepeat(VE_KEY_A)) {
        currentCamera_->moveAlongSide(-cameraSpeed);
    }
}

void VE::World::update(float dt) {
    gui();
    hid(dt);
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

    if (mouseJointSolver_) {
        mouseJointSolver_->applyImpulse(dt, currentCamera_->getPointAlongDirection(10));
    }

    if (jointSolver_) {
        jointSolver_->applyImpulse(dt, Vector(0, 0, 10));
    }

    for (auto &contact: contactSolvers) {
        contact.second.preStep(dt);
    }

    for (int i = 0; i < globalParameters.iterations; i++) {
        for (auto &contact: contactSolvers) {
            contact.second.applyImpulse(dt);
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
    ImGui::Checkbox("polygone", &globalParameters.polygone);
    ImGui::SliderInt("iteration", &globalParameters.iterations, 1, 200);

    static float alfa = 0, beta = 0, gamma = 0;
    ImGui::SliderFloat("x", &alfa, 0.0f, 360.0f);
    ImGui::SliderFloat("y", &beta, 0.0f, 360.0f);
    ImGui::SliderFloat("z", &gamma, 0.0f, 360.0f);
    globalParameters.rotate = Vector(alfa, beta, gamma) / 180.0f * M_PI;
    ImGui::Checkbox("Warnstarting", &globalParameters.warmstarting);

    ImGui::End();

}








