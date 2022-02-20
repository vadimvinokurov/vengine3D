//
// Created by boris on 11/16/2021.
//

#include "ve_world.h"
#include "imgui/imgui.h"
#include "objects/ve_ragdoll_actor.h"
#include "objects/ve_blockjoints_actor.h"
#include "math/ve_splines.h"
#include "animation/ve_gltfloader.h"

using namespace VE;

World::World() {
    resetScene();
}


void World::cubeStacking(const Vector3 &position) {
    auto spawBox = [&](const Transform &transform) {
        RigidBodyPtr body1 = RigidBody::create({create<BoxCollider>(1, 1, 1, 1)});
        body1->setTransform(transform);
        body1->setGravity(Vector3(0.0f, 0.0f, -9.8f));
        worldObjects.push_back(body1);
    };

    for (int i = 0; i < 10; i++) {
        Transform transform;
        transform.position = Vector3(0.5f, 0.5f, 0.6f + 1.2f * static_cast<float>(i)) + position;
        spawBox(transform);
    }
}

void World::sphereStacking(const Vector3 &position) {

    RigidBodyPtr body2 = RigidBody::create(
            {
                    create<SphereCollider>(1, 10, Vector3(0.0f, 0.0f, 0.0f)),
            });

    Transform transform;
    transform.position = Vector3(0, 0, 10.5f) + position;
    body2->setTransform(transform);
    body2->setGravity(Vector3(0.0f, 0.0f, -9.8f));
    worldObjects.push_back(body2);

    RigidBodyPtr body3 = RigidBody::create(
            {
                    create<SphereCollider>(1, 10, Vector3(0.0f, 0.0f, 0.0f)),
            });
    transform.position = Vector3(0, 0, 1.5f) + position;
    body3->setTransform(transform);

    body3->setGravity(Vector3(0.0f, 0.0f, -9.8f));
    worldObjects.push_back(body3);
}

void World::scene() {
    RigidBodyPtr floor = RigidBody::create({create<BoxCollider>(100, 1, 100, 0)});
    floor->setTransform(Transform(Vector3(0, 0, -0.5f)));
    floor->setColor(Color(0.3f, 0.3f, 0.3f));
    worldObjects.push_back(floor);

    cubeStacking(Vector3(-5, -15, 0));
    sphereStacking(Vector3(-10, -15, 0));

    float s = 1.0f;
    float b = 0.8f;
    for (int i = 1; i < 50; i++) {
        RigidBodyPtr stairs = RigidBody::create({create<BoxCollider>(s * b, s * i, 10, 0)});
        stairs->setTransform(Transform(Vector3(s * b * i, 0, s * i / 2)));
        worldObjects.push_back(stairs);
    }

    actors_.push_back(create<RagdollActor>(Vector3(0, 0, 20)));
    actors_.push_back(create<BlockJoints>(Vector3(-15, 7, 10)));


    GLTF gltf = GLTF("../contents/assets/woman/Woman.gltf");
    //GLTF gltf = GLTF("../contents/assets/stormtrooper/scene.gltf");
    if (gltf.good()) {
        restPose = gltf.loadRestPose();
        clips = gltf.loadAnimationClips();
        currentClipNumber = 0;
        currentPose = restPose;

        for (std::size_t i = 0; i < clips.size(); ++i) {
            if (clips[i].getName() == "Walking") {
                currentClipNumber = i;
                break;
            }
        }
    }

}

void World::resetScene() {
    worldObjects.clear();
    contactSolvers.clear();
    actors_.clear();
    scene();
    for (auto &actor: actors_) {
        std::copy(actor->getObjects().begin(), actor->getObjects().end(), std::back_inserter(worldObjects));
    }
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
            float currentLen = (object->centerOfMass() - currentCamera_->getPointAlongDirection(10)).lenSqrt();
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
        currentCamera_->setDirection(mouse_->deltaPosition().y, mouse_->deltaPosition().x * -1);
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


void World::animation(float dt) {
    if (clips.empty()) return;

    playbackTime = clips[currentClipNumber].sample(currentPose, playbackTime + dt);


    std::vector<Vector3> points;
    for (std::size_t i = 0; i < currentPose.jointsCount(); ++i) {
        if (currentPose.getParentIndex(i) == Joint::hasNoParent) {
            continue;
        }

        points.push_back(currentPose.getGlobalTransform(i).position);
        points.push_back(currentPose.getGlobalTransform(currentPose.getParentIndex(i)).position);
    }

    for (auto &&p: points) {
        p *= 0.01;
        p = Vector3(p.x * -1, p.z, p.y) + Vector3(-10, 0, 0);
//        p = p * -1.0f + Vector3(-10, 0, 0);
    }

    for (int i = 0; i < points.size() - 1; i += 2) {
        DebugDraw::Line(points[i], points[i + 1]);
    }
}


void VE::World::update(float dt) {
    gui();
    hid(dt);
    prephysics(dt);
    physics(dt);
    animation(dt);
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

    for (int i = 0; i < globalParameters.iterations; i++) {
        if (mouseJointSolver_) {
            mouseJointSolver_->applyImpulse(dt, currentCamera_->getPointAlongDirection(10));
        }
    }

    for (int i = 0; i < globalParameters.iterations; i++) {
        for (auto &actor:actors_) {
            actor->update(dt);
        }
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
    ImGui::Checkbox("polygone", &globalParameters.polygone);
    ImGui::SliderInt("iteration", &globalParameters.iterations, 1, 200);

    ImGui::SliderFloat("x", &globalParameters.ligthPosition.x, -250.0f, 250.0f);
    ImGui::SliderFloat("y", &globalParameters.ligthPosition.y, -250.0f, 250.0f);
    ImGui::SliderFloat("z", &globalParameters.ligthPosition.z, -250.0f, 250.0f);
    ImGui::Checkbox("Warnstarting", &globalParameters.warmstarting);

    ImGui::End();

}









