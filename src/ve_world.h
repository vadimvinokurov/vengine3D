//
// Created by boris on 11/16/2021.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include <memory>

namespace VE{
    class World {
    public:
        void update(float dt);
    };
    using WorldPtr = std::shared_ptr<World>;
}



#endif //VENGINE3D_WORLD_H
