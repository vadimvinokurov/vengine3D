//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include <memory>
#include "Object/ve_camera.h"
#include "Control/ve_keyboard.hpp"
#include "Control/ve_mouse.hpp"

namespace VE {
    class World {
    public:
        World();
        void update(float dt);
        const Camera &currentCamera();
        void setHid(const VE::KeyboardPtr &keyboard, const VE::MousePtr &mouse);
    private:
        void hid();
        CameraPtr currentCamera_ = std::make_shared<Camera>();

        VE::MousePtr mouse_;
        VE::KeyboardPtr keyboard_;
    };

    using WorldPtr = std::shared_ptr<World>;
}


#endif //VENGINE3D_WORLD_H
