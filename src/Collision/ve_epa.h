//
// Created by boris on 11/29/2021.
//

#ifndef VENGINE3D_VE_EPA_H
#define VENGINE3D_VE_EPA_H

#include "Object/ve_collider.h"

namespace VE {
    class EPA {
    private:
        class UnigueEdge {
        public:
            void add(size_t a, size_t b);
            const std::vector<std::pair<size_t, size_t>> &edges();
        private:
            std::vector<std::pair<size_t, size_t>> edges_;
        };

        struct Face {
            Face(unsigned int i1, unsigned int i2, unsigned int i3) : index{i1, i2, i3} {
            }

            unsigned int index[3] = {0, 0, 0};
            Vector normal;
            float distance = 0.0f;
            bool actualInfo = false;
        };

        class Polytope {
        public:
            Polytope(const std::vector<Vector> &simplex);
            const Face &operator[](size_t i) const;
            void addFace(const Face &face);
            void deleteFace(size_t faceNumber);
            void addVertex(const Vector &vertex);
            size_t minimalDistanceFace() const;

            size_t faceSize() const;
            size_t verticesSize() const;

            void draw();
            void info(std::string s = "");
        private:
            void updateFaceInfo();

            std::vector<Vector> vertices_;
            std::vector<Face> faces_;
        };

    public:
        EPA(const Collider &collider1, const Collider &collider2, const std::vector<Vector> &simplex);
        Vector getResolutionVector();
    private:
        void computeNewFaces(const Vector &support);
        const Collider &collider1_;
        const Collider &collider2_;
        Polytope polytope_;
    };
}

#endif //VENGINE3D_VE_EPA_H
