//
// Created by boris on 12/14/2021.
//

#ifndef VENGINE3D_VE_CONTACT_MANIFOLD_H
#define VENGINE3D_VE_CONTACT_MANIFOLD_H

namespace VE {
    struct ManifoldContactPoint {
        ManifoldContactPoint() {};
        ManifoldContactPoint(VE::Vector p, VE::Vector n, float collDepth) : point(p), normal(n), collisionDepth(collDepth) {};
        VE::Vector point;
        VE::Vector normal;
        VE::Vector tangent1;
        VE::Vector tangent2;
        float collisionDepth = 0.0f;

        float normalImpulse = 0.0f;
        float tangent1Impulse = 0.0f;
        float tangent2Impulse = 0.0f;
        float pseudoImpulse = 0.0f;

        float normalEffectiveMass = 0.0f;
        float tangentEffectiveMass = 0.0f;
        float normalInitRelativeVelocity = 0.0f;
        float tangentInitRelativeVelocity = 0.0f;
        float bias = 0.0f;
        bool saved = false;
    };

    using ContactMainfold = std::vector<ManifoldContactPoint>;
}


#endif //VENGINE3D_VE_CONTACT_MANIFOLD_H
