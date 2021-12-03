//
// Created by boris on 12/2/2021.
//

#ifndef VENGINE3D_VE_MINKOWSKI_SUM_H
#define VENGINE3D_VE_MINKOWSKI_SUM_H

#include "objects/ve_collider.h"

namespace VE{
    void minkowskiSumPoint(const Collider &collider1, const Collider &collider2){
        for(const Vector &vertex1: collider1.vertices()){
            for(const Vector &vertex2: collider2.vertices()){
                (collider1.transform_.apply(vertex1) - collider2.transform_.apply(vertex2)).drawPoint();
            }
        }
    }
}

#endif //VENGINE3D_VE_MINKOWSKI_SUM_H
