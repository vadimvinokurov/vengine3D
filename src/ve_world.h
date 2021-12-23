//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include <memory>
#include "physics/ve_contact_solver.h"
#include "physics/ve_mouse_joint_solver.h"
#include "objects/ve_rigid_body.h"
#include "objects/ve_camera.h"
#include "control/ve_keyboard.h"
#include "control/ve_mouse.h"

namespace VE {
    class World {
    public:
        World();
        void update(float dt);

        const Camera &currentCamera();
        void setHid(const VE::KeyboardPtr &keyboard, const VE::MousePtr &mouse);
        std::vector<VE::RigidBodyPtr> worldObjects;
    private:
        void hid(float dt);
        void cameraControl(float dt);
        void gui();

        void prephysics(float dt);
        void physics(float dt);

        void resetScene();
        CameraPtr currentCamera_ = std::make_shared<Camera>();
        std::map<VE::ContactKey, VE::ContactSolver> contactSolvers;

        VE::MousePtr mouse_;
        VE::KeyboardPtr keyboard_;
        VE::RigidBodyPtr mouseObject_;
        VE::MouseJointSolverPtr mouseJointSolver_;
    };

    using WorldPtr = std::shared_ptr<World>;
}


#endif //VENGINE3D_WORLD_H
