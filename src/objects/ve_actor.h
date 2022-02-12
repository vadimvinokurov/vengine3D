//
// Created by boris on 1/11/2022.
//

#ifndef VENGINE3D_VE_ACTOR_H
#define VENGINE3D_VE_ACTOR_H

#include "ve_colliders.h"
#include "ve_rigid_body.h"
#include "physics/ve_ball_socket_joint_solver.h"
#include "ve_utils.h"

namespace VE {
    class Actor {
    public:
        Actor() {}

        virtual void update(float dt) {
            for (auto &joint:joints_) {
                joint.applyImpulse(dt);
            }
        }

        virtual std::vector<VE::RigidBodyPtr> &getObjects() {
            return bodys_;
        }

        virtual std::vector<VE::BallAndSocketJointSolver> &getJoints() {
            return joints_;
        }

    protected:
        std::vector<VE::RigidBodyPtr> bodys_;
        std::vector<VE::BallAndSocketJointSolver> joints_;
    };

    using ActorPtr = std::shared_ptr<VE::Actor>;
}


#endif //VENGINE3D_VE_ACTOR_H
