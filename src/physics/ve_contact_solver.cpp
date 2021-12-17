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


                if ((localCPointOb1 - newLocalCPointOb1).sqrtAbs() < ContactSolverParametrs::contactSlop &&
                    (localCPointOb2 - newLocalCPointOb2).sqrtAbs() < ContactSolverParametrs::contactSlop &&
                    localNormalOb1.dot(newLocalNormalOb1) > ContactSolverParametrs::normalDeviation &&
                    localNormalOb2.dot(newLocalNormalOb2) >ContactSolverParametrs:: normalDeviation) {

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


float ContactSolver::computeEffectiveMass(const Vector &J0, const Vector &J1, const Vector &J2, const Vector &J3) {
    return 0;
}

void ContactSolver::preStep(float dt) {
    for (ManifoldContactPoint &contact: contactMainfold_) {

        Vector r1 = contact.point - body1.centerOfMass();
        Vector r2 = contact.point - body2.centerOfMass();
        VE::Vector relativeVelocity = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        contact.bias = -ContactSolverParametrs::beta / dt * std::max(0.0f, contact.collisionDepth - ContactSolverParametrs::penetrationSlop);

        Vector nCrossR1 = contact.normal * r1;
        Vector nCrossR2 = contact.normal * r2;
        contact.normalEffectiveMass = 1 / (body1.invMass() + body2.invMass() +
                                                (nCrossR1 * body1.invInertia()).dot(nCrossR1) +
                                                (nCrossR2 * body2.invInertia()).dot(nCrossR2));
        contact.normalInitRelativeVelocity = std::min(relativeVelocity.dot(contact.normal) + ContactSolverParametrs::speedSlop, 0.0f);


        contact.tangent1 = (contact.normal * (contact.normal * relativeVelocity)).normolize();
        Vector t1CrossR1 = contact.tangent1 * r1;
        Vector t1CrossR2 = contact.tangent1 * r2;
        contact.tangent1EffectiveMass = 1 / (body1.invMass() + body2.invMass() +
                                                  (t1CrossR1 * body1.invInertia()).dot(t1CrossR1) +
                                                  (t1CrossR2 * body2.invInertia()).dot(t1CrossR2));
        contact.tangent1InitRelativeVelocity = relativeVelocity.dot(contact.tangent1);


        contact.tangent2 = contact.normal * contact.tangent1;
        Vector t2CrossR1 = contact.tangent2 * r1;
        Vector t2CrossR2 = contact.tangent2 * r2;
        contact.tangent2EffectiveMass = 1 / (body1.invMass() + body2.invMass() +
                                                  (t2CrossR1 * body1.invInertia()).dot(t2CrossR1) +
                                                  (t2CrossR2 * body2.invInertia()).dot(t2CrossR2));
        contact.tangent2InitRelativeVelocity = relativeVelocity.dot(contact.tangent2);


        Vector L = contact.normal * contact.normalImpulse + contact.tangent1 * contact.tangent1Impulse +
                contact.tangent2 * contact.tangent2Impulse;
        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));
        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));


        L = contact.normal * contact.pseudoImpulse;
        if (globalParameters.pseudoVelosity) {
            body1.setPseudoLinearVelocity(body1.pseudoLinearVelocity() - L * body1.invMass());
            body1.setPseudoAngularVelocity(body1.pseudoAngularVelocity() - body1.invInertia() * (r1 * L));

            body2.setPseudoLinearVelocity(body2.pseudoLinearVelocity() + L * body2.invMass());
            body2.setPseudoAngularVelocity(body2.pseudoAngularVelocity() + body2.invInertia() * (r2 * L));
        }
    }
}


void ContactSolver::applyImpulse(float dt) {
    for (ManifoldContactPoint &contact: contactMainfold_) {
        Vector r1 = contact.point - body1.centerOfMass();
        Vector r2 = contact.point - body2.centerOfMass();
        Vector vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        float vn = vrel.dot(contact.normal);
        float dPn = contact.normalEffectiveMass * -(vn + restitution_ * contact.normalInitRelativeVelocity);
        float oldPn = contact.normalImpulse;
        contact.normalImpulse = std::max(contact.normalImpulse + dPn, 0.0f);
        dPn = contact.normalImpulse - oldPn;
        Vector L = contact.normal * dPn;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));


        vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;
        float vt = vrel.dot(contact.tangent1);
        float dPt = contact.tangent1EffectiveMass * -(vt + restitution_ * contact.tangent1InitRelativeVelocity);
        float oldPt = contact.tangent1Impulse;
        contact.tangent1Impulse = std::clamp(contact.tangent1Impulse + dPt, -contact.normalImpulse * friction_,
                                                  contact.normalImpulse * friction_);
        dPt = contact.tangent1Impulse - oldPt;
        L = contact.tangent1 * dPt;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));


        vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;
        vt = vrel.dot(contact.tangent2);
        dPt = contact.tangent2EffectiveMass * -(vt + restitution_ * contact.tangent2InitRelativeVelocity);
        oldPt = contact.tangent2Impulse;
        contact.tangent2Impulse = std::clamp(contact.tangent2Impulse + dPt, -contact.normalImpulse * friction_,
                                             contact.normalImpulse * friction_);
        dPt = contact.tangent2Impulse - oldPt;
        L = contact.tangent2 * dPt;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));
    }
}

void ContactSolver::applyPseudoImpulse(float dt) {
    if (globalParameters.pseudoVelosity) {
        for (ManifoldContactPoint &contact: contactMainfold_) {
            Vector r1 = contact.point - body1.centerOfMass();
            Vector r2 = contact.point - body2.centerOfMass();

            Vector vrel = body2.pseudoLinearVelocity() + body2.pseudoAngularVelocity() * r2 - body1.pseudoLinearVelocity() -
                          body1.pseudoAngularVelocity() * r1;

            float vn = vrel.dot(contact.normal);

            float dPn = contact.normalEffectiveMass * -(vn + contact.bias);
            float oldPn = contact.pseudoImpulse;
            contact.pseudoImpulse = std::max(contact.pseudoImpulse + dPn, 0.0f);
            dPn = contact.pseudoImpulse - oldPn;

            Vector L = contact.normal * dPn;

            body1.setPseudoLinearVelocity(body1.pseudoLinearVelocity() - L * body1.invMass());
            body1.setPseudoAngularVelocity(body1.pseudoAngularVelocity() - body1.invInertia() * (r1 * L));

            body2.setPseudoLinearVelocity(body2.pseudoLinearVelocity() + L * body2.invMass());
            body2.setPseudoAngularVelocity(body2.pseudoAngularVelocity() + body2.invInertia() * (r2 * L));
        }
    }
}

