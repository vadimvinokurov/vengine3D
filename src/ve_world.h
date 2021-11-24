//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include <memory>
#include "Object/ve_camera.h"
#include "Control/ve_keyboard.h"
#include "Control/ve_mouse.h"

namespace VE {
    class World {
    public:
        World();
        void update(float dt);

        const Camera &currentCamera();
        void setHid(const VE::KeyboardPtr &keyboard, const VE::MousePtr &mouse);
    private:
        void hid();
        void hid_CameraControl();
        void hid_PositionControl();
        CameraPtr currentCamera_ = std::make_shared<Camera>();

        VE::MousePtr mouse_;
        VE::KeyboardPtr keyboard_;

        const float mouseSense = 50.0f;
    };

    using WorldPtr = std::shared_ptr<World>;
}


#endif //VENGINE3D_WORLD_H
