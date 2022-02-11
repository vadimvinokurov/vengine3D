//
// Created by Boris on 10/21/2021.
//

#include "ve_contact_solver.h"

using namespace VE;


ContactSolver::ContactSolver(RigidBody &body1, RigidBody &body2) : body1(body1), body2(body2) {
    contact_ = getContactMainfold(body1, body2, contactMainfold_);
    restitution_ = (body1.restitution() + body2.restitution()) / 2;
    friction_ = (body1.friction() + body2.friction()) / 2;
}

void ContactSolver::update(ContactMainfold newContactMainfold) {
    if (globalParameters.warmstarting) {
        for (ManifoldContactPoint &newContactPoint: newContactMainfold) {
            for (ManifoldContactPoint &contactPoint: contactMainfold_) {

                Vector3 localContactPointOb1 = body1.globalToLocalPoint(contactPoint.point);
                Vector3 localContactPointOb2 = body2.globalToLocalPoint(contactPoint.point);

                Vector3 newLocalContactPointOb1 = body1.globalToLocalPoint(newContactPoint.point);
                Vector3 newLocalContactPointOb2 = body2.globalToLocalPoint(newContactPoint.point);

                Vector3 localNormalOb1 = body1.globalToLocalPoint(contactPoint.point + contactPoint.normal) - localContactPointOb1;
                Vector3 localNormalOb2 = body2.globalToLocalPoint(contactPoint.point + contactPoint.normal) - localContactPointOb2;

                Vector3 newLocalNormalOb1 = body1.globalToLocalPoint(newContactPoint.point + newContactPoint.normal) - newLocalContactPointOb1;
                Vector3 newLocalNormalOb2 = body2.globalToLocalPoint(newContactPoint.point + newContactPoint.normal) - newLocalContactPointOb2;

                if ((localContactPointOb1 - newLocalContactPointOb1).lenSqrt() < ContactSolverParametrs::contactSlop &&
                        (localContactPointOb2 - newLocalContactPointOb2).lenSqrt() < ContactSolverParametrs::contactSlop &&
                    localNormalOb1.dot(newLocalNormalOb1) > ContactSolverParametrs::normalDeviation &&
                    localNormalOb2.dot(newLocalNormalOb2) > ContactSolverParametrs::normalDeviation) {

                    newContactPoint.normalImpulse = contactPoint.normalImpulse;
                    newContactPoint.tangent1Impulse = contactPoint.tangent1Impulse;
                    newContactPoint.tangent2Impulse = contactPoint.tangent2Impulse;
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
    for (ManifoldContactPoint &contact: contactMainfold_) {

        Vector3 r1 = contact.point - body1.centerOfMass();
        Vector3 r2 = contact.point - body2.centerOfMass();

        VE::Vector3 relativeVelocity = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        contact.tangent1 = (contact.normal * (contact.normal * relativeVelocity)).getNormalized();
        contact.tangent2 = (contact.normal * contact.tangent1).getNormalized();

        contact.normalEffectiveMass = computeEffectiveMass(contact.normal * r1, contact.normal * r2);
        contact.tangent1EffectiveMass = computeEffectiveMass(contact.tangent1 * r1, contact.tangent1 * r2);
        contact.tangent2EffectiveMass = computeEffectiveMass(contact.tangent2 * r1, contact.tangent2 * r2);

        contact.normalInitRelativeVelocity = std::min(0.0f, relativeVelocity.dot(contact.normal) + ContactSolverParametrs::speedSlop);
        contact.tangent1InitRelativeVelocity = relativeVelocity.dot(contact.tangent1);
        contact.tangent2InitRelativeVelocity = relativeVelocity.dot(contact.tangent2);

        contact.bias = -ContactSolverParametrs::beta / dt * std::max(0.0f, contact.collisionDepth - ContactSolverParametrs::penetrationSlop);


        Vector3 L = contact.normal * contact.normalImpulse;
        L = L + contact.tangent1 * contact.tangent1Impulse + contact.tangent2 * contact.tangent2Impulse;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));
        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));

        Vector3 pseudoL = contact.normal * contact.pseudoImpulse;
        body1.setPseudoLinearVelocity(body1.pseudoLinearVelocity() - pseudoL * body1.invMass());
        body1.setPseudoAngularVelocity(body1.pseudoAngularVelocity() - body1.invInertia() * (r1 * pseudoL));

        body2.setPseudoLinearVelocity(body2.pseudoLinearVelocity() + pseudoL * body2.invMass());
        body2.setPseudoAngularVelocity(body2.pseudoAngularVelocity() + body2.invInertia() * (r2 * pseudoL));
    }
}

float ContactSolver::computeEffectiveMass(const Vector3 &nCrossR1, const Vector3 &nCrossR2) {
    return 1 / (body1.invMass() + body2.invMass() +
                (nCrossR1 * body1.invInertia()).dot(nCrossR1) +
                (nCrossR2 * body2.invInertia()).dot(nCrossR2));
}

void ContactSolver::applyImpulse(float dt) {
    for (ManifoldContactPoint &contact: contactMainfold_) {
        Vector3 r1 = contact.point - body1.centerOfMass();
        Vector3 r2 = contact.point - body2.centerOfMass();
        Vector3 vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        float dPn = contact.normalEffectiveMass * -(vrel.dot(contact.normal) + restitution_ * contact.normalInitRelativeVelocity);
        float oldPn = contact.normalImpulse;
        contact.normalImpulse = std::max(contact.normalImpulse + dPn, 0.0f);
        Vector3 Ln = contact.normal * (contact.normalImpulse - oldPn);


        float dPt1 = contact.tangent1EffectiveMass * -(vrel.dot(contact.tangent1) + restitution_ * contact.tangent1InitRelativeVelocity);
        float oldPt1 = contact.tangent1Impulse;
        contact.tangent1Impulse = std::clamp(contact.tangent1Impulse + dPt1, -contact.normalImpulse * friction_,
                                             contact.normalImpulse * friction_);
        Vector3 Lt1 = contact.tangent1 * (contact.tangent1Impulse - oldPt1);


        float dPt2 = contact.tangent2EffectiveMass * -(vrel.dot(contact.tangent2) + restitution_ * contact.tangent2InitRelativeVelocity);
        float oldPt2 = contact.tangent2Impulse;
        contact.tangent2Impulse = std::clamp(contact.tangent2Impulse + dPt2, -contact.normalImpulse * friction_,
                                             contact.normalImpulse * friction_);
        Vector3 Lt2 = contact.tangent2 * (contact.tangent2Impulse - oldPt2);


        Vector3 L = Ln + Lt1 + Lt2;
        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));
    }
}

void ContactSolver::applyPseudoImpulse(float dt) {
    for (ManifoldContactPoint &contact: contactMainfold_) {
        Vector3 r1 = contact.point - body1.centerOfMass();
        Vector3 r2 = contact.point - body2.centerOfMass();

        Vector3 vrel = body2.pseudoLinearVelocity() + body2.pseudoAngularVelocity() * r2 - body1.pseudoLinearVelocity() -
                      body1.pseudoAngularVelocity() * r1;

        float vn = vrel.dot(contact.normal);

        float dPn = contact.normalEffectiveMass * -(vn + contact.bias);
        float oldPn = contact.pseudoImpulse;
        contact.pseudoImpulse = std::max(contact.pseudoImpulse + dPn, 0.0f);
        dPn = contact.pseudoImpulse - oldPn;

        Vector3 L = contact.normal * dPn;

        body1.setPseudoLinearVelocity(body1.pseudoLinearVelocity() - L * body1.invMass());
        body1.setPseudoAngularVelocity(body1.pseudoAngularVelocity() - body1.invInertia() * (r1 * L));

        body2.setPseudoLinearVelocity(body2.pseudoLinearVelocity() + L * body2.invMass());
        body2.setPseudoAngularVelocity(body2.pseudoAngularVelocity() + body2.invInertia() * (r2 * L));
    }
}

bool ContactSolver::isCollide() {
    return contact_;
}

const ContactMainfold &ContactSolver::contactMainfold() {
    return contactMainfold_;
}
