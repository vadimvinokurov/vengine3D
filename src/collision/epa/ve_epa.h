//
// Created by boris on 11/29/2021.
//

#ifndef VENGINE3D_VE_EPA_H
#define VENGINE3D_VE_EPA_H

#include "objects/ve_collider.h"
#include "ve_epa_polytope.h"
#include "ve_epa_unique_edges.h"

namespace VE {
    namespace EPA {
        class PenetrationDepth{
        public:
            PenetrationDepth(const Collider &collider1, const Collider &collider2, const std::vector<Vector> &simplex);
            Vector getVector();
        private:
            void computeNewFaces(const Vector &support);
            const Collider &collider1_;
            const Collider &collider2_;
            Polytope polytope_;


            struct debugInfo {
                Polytope polytope;
                Vector support;
                Vector minNormal;
                std::vector<float> direction;
            };
            std::vector<debugInfo> polytopeStage;
        };
    }
}

#endif //VENGINE3D_VE_EPA_H
