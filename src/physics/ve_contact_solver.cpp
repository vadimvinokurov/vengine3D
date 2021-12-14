//
// Created by Boris on 10/21/2021.
//

#include "ve_contact_solver.h"
#include "collision/ve_collision.h"

using namespace VE;


ContactSolver::ContactSolver(RigidBody &body1, RigidBody &body2) : body1(body1), body2(body2) {
    this->contact = testIntersection(body1, body2, contactMainfold_);
    restitution_ = (body1.restitution() + body2.restitution()) / 2;
    friction_ = (body1.friction() + body2.friction()) / 2;
}

void ContactSolver::update(ContactMainfold newContactMainfold) {
    if (globalParameters.warmstarting) {
        for (ManifoldContactPoint &newContactPoint: newContactMainfold) {
            for (ManifoldContactPoint &contactPoint: contactMainfold_) {

                VE::Vector localCPointOb1 = body1.globalToLocalPoint(contactPoint.point);
                VE::Vector localCPointOb2 = body2.globalToLocalPoint(contactPoint.point);

                VE::Vector newLocalCPointOb1 = body1.globalToLocalPoint(newContactPoint.point);
                VE::Vector newLocalCPointOb2 = body2.globalToLocalPoint(newContactPoint.point);


                VE::Vector localNormalOb1 = body1.globalToLocalPoint(contactPoint.point + contactPoint.normal) - localCPointOb1;
                VE::Vector localNormalOb2 = body2.globalToLocalPoint(contactPoint.point + contactPoint.normal) - localCPointOb2;

                VE::Vector newLocalNormalOb1 = body1.globalToLocalPoint(newContactPoint.point + newContactPoint.normal) - newLocalCPointOb1;
                VE::Vector newLocalNormalOb2 = body2.globalToLocalPoint(newContactPoint.point + newContactPoint.normal) - newLocalCPointOb2;


                if ((localCPointOb1 - newLocalCPointOb1).sqrtAbs() < 0.01f &&
                    (localCPointOb2 - newLocalCPointOb2).sqrtAbs() < 0.01f &&
                    localNormalOb1.dot(newLocalNormalOb1) > 0.99f &&
                    localNormalOb2.dot(newLocalNormalOb2) > 0.99f) {

                    newContactPoint.normalImpulse = contactPoint.normalImpulse;
                    newContactPoint.tangent1Impulse = contactPoint.tangent1Impulse;
                    newContactPoint.pseudoImpulse = contactPoint.pseudoImpulse;
                    newContactPoint.saved = true;
                    break;
                }
            }
        }
    }
    contactMainfold_ = newContactMainfold;
}


void ContactSolver::preStep(float dt) {
    for (ManifoldContactPoint &contactPoint: contactMainfold_) {

        Vector r1 = contactPoint.point - body1.centerOfMass();
        Vector r2 = contactPoint.point - body2.centerOfMass();
        VE::Vector relativeVelocity = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        contactPoint.bias = -beta / dt * std::max(0.0f, contactPoint.collisionDepth - penetrationSlop);

        Vector nCrossR1 = contactPoint.normal * r1;
        Vector nCrossR2 = contactPoint.normal * r2;
        contactPoint.normalEffectiveMass = 1 / (body1.invMass() + body2.invMass() +
                                                (nCrossR1 * body1.invInertia()).dot(nCrossR1) +
                                                (nCrossR2 * body2.invInertia()).dot(nCrossR2));
        contactPoint.normalInitRelativeVelocity = std::min(relativeVelocity.dot(contactPoint.normal) + speedSlop, 0.0f);


        contactPoint.tangent1 = (contactPoint.normal * (contactPoint.normal * relativeVelocity)).normolize();
        Vector t1CrossR1 = contactPoint.tangent1 * r1;
        Vector t1CrossR2 = contactPoint.tangent1 * r2;
        contactPoint.tangent1EffectiveMass = 1 / (body1.invMass() + body2.invMass() +
                                                  (t1CrossR1 * body1.invInertia()).dot(t1CrossR1) +
                                                  (t1CrossR2 * body2.invInertia()).dot(t1CrossR2));
        contactPoint.tangent1InitRelativeVelocity = relativeVelocity.dot(contactPoint.tangent1);


        contactPoint.tangent2 = contactPoint.normal * contactPoint.tangent1;
        Vector t2CrossR1 = contactPoint.tangent2 * r1;
        Vector t2CrossR2 = contactPoint.tangent2 * r2;
        contactPoint.tangent2EffectiveMass = 1 / (body1.invMass() + body2.invMass() +
                                                  (t2CrossR1 * body1.invInertia()).dot(t2CrossR1) +
                                                  (t2CrossR2 * body2.invInertia()).dot(t2CrossR2));
        contactPoint.tangent2InitRelativeVelocity = relativeVelocity.dot(contactPoint.tangent2);


        Vector L = contactPoint.normal * contactPoint.normalImpulse + contactPoint.tangent1 * contactPoint.tangent1Impulse +
                   contactPoint.tangent2 * contactPoint.tangent2Impulse;
        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));
        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));


        L = contactPoint.normal * contactPoint.pseudoImpulse;
        if (globalParameters.pseudoVelosity) {
            body1.setPseudoLinearVelocity(body1.pseudoLinearVelocity() - L * body1.invMass());
            body1.setPseudoAngularVelocity(body1.pseudoAngularVelocity() - body1.invInertia() * (r1 * L));

            body2.setPseudoLinearVelocity(body2.pseudoLinearVelocity() + L * body2.invMass());
            body2.setPseudoAngularVelocity(body2.pseudoAngularVelocity() + body2.invInertia() * (r2 * L));
        }
    }
}


void ContactSolver::applyImpulse(float dt) {
    for (ManifoldContactPoint &contactPoint: contactMainfold_) {
        Vector r1 = contactPoint.point - body1.centerOfMass();
        Vector r2 = contactPoint.point - body2.centerOfMass();
        Vector vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        float vn = vrel.dot(contactPoint.normal);
        float dPn = contactPoint.normalEffectiveMass * -(vn + restitution_ * contactPoint.normalInitRelativeVelocity);
        float oldPn = contactPoint.normalImpulse;
        contactPoint.normalImpulse = std::max(contactPoint.normalImpulse + dPn, 0.0f);
        dPn = contactPoint.normalImpulse - oldPn;
        Vector L = contactPoint.normal * dPn;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));


        vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;
        float vt = vrel.dot(contactPoint.tangent1);
        float dPt = contactPoint.tangent1EffectiveMass * -(vt + restitution_ * contactPoint.tangent1InitRelativeVelocity);
        float oldPt = contactPoint.tangent1Impulse;
        contactPoint.tangent1Impulse = std::clamp(contactPoint.tangent1Impulse + dPt, -contactPoint.normalImpulse * friction_,
                                                  contactPoint.normalImpulse * friction_);
        dPt = contactPoint.tangent1Impulse - oldPt;
        L = contactPoint.tangent1 * dPt;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));


        vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;
        vt = vrel.dot(contactPoint.tangent2);
        dPt = contactPoint.tangent2EffectiveMass * -(vt + restitution_ * contactPoint.tangent2InitRelativeVelocity);
        oldPt = contactPoint.tangent2Impulse;
        contactPoint.tangent2Impulse = std::clamp(contactPoint.tangent2Impulse + dPt, -contactPoint.normalImpulse * friction_,
                                                  contactPoint.normalImpulse * friction_);
        dPt = contactPoint.tangent2Impulse - oldPt;
        L = contactPoint.tangent2 * dPt;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));
    }
}

void ContactSolver::applyPseudoImpulse(float dt) {
    if (globalParameters.pseudoVelosity) {
        for (ManifoldContactPoint &contactPoint: contactMainfold_) {
            Vector r1 = contactPoint.point - body1.centerOfMass();
            Vector r2 = contactPoint.point - body2.centerOfMass();

            Vector vrel = body2.pseudoLinearVelocity() + body2.pseudoAngularVelocity() * r2 - body1.pseudoLinearVelocity() -
                          body1.pseudoAngularVelocity() * r1;

            float vn = vrel.dot(contactPoint.normal);

            float dPn = contactPoint.normalEffectiveMass * -(vn + contactPoint.bias);
            float oldPn = contactPoint.pseudoImpulse;
            contactPoint.pseudoImpulse = std::max(contactPoint.pseudoImpulse + dPn, 0.0f);
            dPn = contactPoint.pseudoImpulse - oldPn;

            Vector L = contactPoint.normal * dPn;

            body1.setPseudoLinearVelocity(body1.pseudoLinearVelocity() - L * body1.invMass());
            body1.setPseudoAngularVelocity(body1.pseudoAngularVelocity() - body1.invInertia() * (r1 * L));

            body2.setPseudoLinearVelocity(body2.pseudoLinearVelocity() + L * body2.invMass());
            body2.setPseudoAngularVelocity(body2.pseudoAngularVelocity() + body2.invInertia() * (r2 * L));
        }
    }
}
