//
// Created by boris on 12/3/2021.
//

#ifndef VENGINE3D_VE_EPA_POLYTOPE_H
#define VENGINE3D_VE_EPA_POLYTOPE_H

#include "ve_epa_face.h"
#include "math/ve_vector.h"
#include "ve_stdlibs.h"

namespace VE{
    namespace EPA{
        class Polytope {
        public:
            Polytope(const std::vector<Vector3> &simplex);
            Polytope(const Polytope& polytope) = default;
            Polytope& operator=(const Polytope& polytope) = default;

            void addFace(const Face &face);
            void deleteFace(size_t faceNumber);
            size_t getFacesSize() const;
            const Face &operator[](size_t faceNumber) const;

            void addVertex(const Vector3 &vertex);
            const Vector3& getFaceVertex(size_t faceNumber, size_t vertexNumber) const;
            size_t getVerticesSize() const;

            const Face &getClosestFaceToOrigin() const;

            void debugDraw(const Color &color = Color(0.5f, 0.5f, 0.5f));
        private:
            void updateFacesData();

            std::vector<Vector3> vertices_;
            std::vector<Face> faces_;
        };
    }
}

#endif //VENGINE3D_VE_EPA_POLYTOPE_H
