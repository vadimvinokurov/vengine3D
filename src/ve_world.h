//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include <memory>
#include "Object/ve_camera.h"

namespace VE {
    class World {
    public:
        World();
        void update(float dt);
        const Camera &currentCamera();
    private:
        CameraPtr currentCamera_ = std::make_shared<Camera>();
    };

    using WorldPtr = std::shared_ptr<World>;
}


#endif //VENGINE3D_WORLD_H
