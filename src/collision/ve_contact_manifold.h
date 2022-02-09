//
// Created by boris on 12/14/2021.
//

#ifndef VENGINE3D_VE_CONTACT_MANIFOLD_H
#define VENGINE3D_VE_CONTACT_MANIFOLD_H

namespace VE {
    struct ManifoldContactPoint {
        ManifoldContactPoint() {};
        ManifoldContactPoint(VE::Vector3 p, VE::Vector3 n, float collDepth) : point(p), normal(n), collisionDepth(collDepth) {};
        VE::Vector3 point;
        VE::Vector3 normal;
        VE::Vector3 tangent1;
        VE::Vector3 tangent2;
        float collisionDepth = 0.0f;

        float normalImpulse = 0.0f;
        float tangent1Impulse = 0.0f;
        float tangent2Impulse = 0.0f;
        float pseudoImpulse = 0.0f;

        float normalEffectiveMass = 0.0f;
        float tangent1EffectiveMass = 0.0f;
        float tangent2EffectiveMass = 0.0f;

        float normalInitRelativeVelocity = 0.0f;
        float tangent1InitRelativeVelocity = 0.0f;
        float tangent2InitRelativeVelocity = 0.0f;


        float bias = 0.0f;
        bool saved = false;
    };

    using ContactMainfold = std::vector<ManifoldContactPoint>;
}


#endif //VENGINE3D_VE_CONTACT_MANIFOLD_H
