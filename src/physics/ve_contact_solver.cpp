//
// Created by Boris on 10/21/2021.
//

#include "ve_contact_solver.h"
#include "collision/ve_collision.h"

using namespace VE;


ContactSolver::ContactSolver(RigidBody &body1, RigidBody &body2) : body1(body1), body2(body2) {
    contact_ = testIntersection(body1, body2, contactMainfold_);
    restitution_ = (body1.restitution() + body2.restitution()) / 2;
    friction_ = (body1.friction() + body2.friction()) / 2;
}

void ContactSolver::update(ContactMainfold newContactMainfold) {
    if (globalParameters.warmstarting) {
        for (ManifoldContactPoint &newContactPoint: newContactMainfold) {
            for (ManifoldContactPoint &contactPoint: contactMainfold_) {

                Vector localContactPointOb1 = body1.globalToLocalPoint(contactPoint.point);
                Vector localContactPointOb2 = body2.globalToLocalPoint(contactPoint.point);

                Vector newLocalContactPointOb1 = body1.globalToLocalPoint(newContactPoint.point);
                Vector newLocalContactPointOb2 = body2.globalToLocalPoint(newContactPoint.point);

                Vector localNormalOb1 = body1.globalToLocalPoint(contactPoint.point + contactPoint.normal) - localContactPointOb1;
                Vector localNormalOb2 = body2.globalToLocalPoint(contactPoint.point + contactPoint.normal) - localContactPointOb2;

                Vector newLocalNormalOb1 = body1.globalToLocalPoint(newContactPoint.point + newContactPoint.normal) - newLocalContactPointOb1;
                Vector newLocalNormalOb2 = body2.globalToLocalPoint(newContactPoint.point + newContactPoint.normal) - newLocalContactPointOb2;

                if ((localContactPointOb1 - newLocalContactPointOb1).sqrtAbs() < ContactSolverParametrs::contactSlop &&
                    (localContactPointOb2 - newLocalContactPointOb2).sqrtAbs() < ContactSolverParametrs::contactSlop &&
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

        Vector r1 = contact.point - body1.centerOfMass();
        Vector r2 = contact.point - body2.centerOfMass();

        VE::Vector relativeVelocity = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        contact.tangent1 = (contact.normal * (contact.normal * relativeVelocity)).normolize();
        contact.tangent2 = (contact.normal * contact.tangent1).normolize();

        contact.normalEffectiveMass = computeEffectiveMass(contact.normal * r1, contact.normal * r2);
        contact.tangent1EffectiveMass = computeEffectiveMass(contact.tangent1 * r1, contact.tangent1 * r2);
        contact.tangent2EffectiveMass = computeEffectiveMass(contact.tangent2 * r1, contact.tangent2 * r2);

        contact.normalInitRelativeVelocity = std::min(0.0f, relativeVelocity.dot(contact.normal) + ContactSolverParametrs::speedSlop);
        contact.tangent1InitRelativeVelocity = relativeVelocity.dot(contact.tangent1);
        contact.tangent2InitRelativeVelocity = relativeVelocity.dot(contact.tangent2);

        contact.bias = -ContactSolverParametrs::beta / dt * std::max(0.0f, contact.collisionDepth - ContactSolverParametrs::penetrationSlop);


        Vector L = contact.normal * contact.normalImpulse;
        L = L + contact.tangent1 * contact.tangent1Impulse + contact.tangent2 * contact.tangent2Impulse;

        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));
        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));

        Vector pseudoL = contact.normal * contact.pseudoImpulse;
        body1.setPseudoLinearVelocity(body1.pseudoLinearVelocity() - pseudoL * body1.invMass());
        body1.setPseudoAngularVelocity(body1.pseudoAngularVelocity() - body1.invInertia() * (r1 * pseudoL));

        body2.setPseudoLinearVelocity(body2.pseudoLinearVelocity() + pseudoL * body2.invMass());
        body2.setPseudoAngularVelocity(body2.pseudoAngularVelocity() + body2.invInertia() * (r2 * pseudoL));


        contact.point.drawPoint(6, Color(1, 0, 0));
//        r2.draw(body2.centerOfMass());
//        contact.normal.draw(contact.point, Color(0, 0, 1));
////        contact.tangent1.draw(contact.point);
////        contact.tangent2.draw(contact.point, Color(0, 1, 0));
//        Vector r = relativeVelocity.normolize();
//        r.draw(contact.point);
//        body2.angularVelocity().draw(body2.centerOfMass(), Color(0, 1, 0));
    }
}

float ContactSolver::computeEffectiveMass(const Vector &nCrossR1, const Vector &nCrossR2) {
    return 1 / (body1.invMass() + body2.invMass() +
                (nCrossR1 * body1.invInertia()).dot(nCrossR1) +
                (nCrossR2 * body2.invInertia()).dot(nCrossR2));
}

void ContactSolver::applyImpulse(float dt) {
    for (ManifoldContactPoint &contact: contactMainfold_) {
        Vector r1 = contact.point - body1.centerOfMass();
        Vector r2 = contact.point - body2.centerOfMass();
        Vector vrel = body2.linearVelocity() + body2.angularVelocity() * r2 - body1.linearVelocity() - body1.angularVelocity() * r1;

        float dPn = contact.normalEffectiveMass * -(vrel.dot(contact.normal) + restitution_ * contact.normalInitRelativeVelocity);
        float oldPn = contact.normalImpulse;
        contact.normalImpulse = std::max(contact.normalImpulse + dPn, 0.0f);
        Vector Ln = contact.normal * (contact.normalImpulse - oldPn);


        float dPt1 = contact.tangent1EffectiveMass * -(vrel.dot(contact.tangent1) + restitution_ * contact.tangent1InitRelativeVelocity);
        float oldPt1 = contact.tangent1Impulse;
        contact.tangent1Impulse = std::clamp(contact.tangent1Impulse + dPt1, -contact.normalImpulse * friction_,
                                             contact.normalImpulse * friction_);
        Vector Lt1 = contact.tangent1 * (contact.tangent1Impulse - oldPt1);


        float dPt2 = contact.tangent2EffectiveMass * -(vrel.dot(contact.tangent2) + restitution_ * contact.tangent2InitRelativeVelocity);
        float oldPt2 = contact.tangent2Impulse;
        contact.tangent2Impulse = std::clamp(contact.tangent2Impulse + dPt2, -contact.normalImpulse * friction_,
                                             contact.normalImpulse * friction_);
        Vector Lt2 = contact.tangent2 * (contact.tangent2Impulse - oldPt2);


        Vector L = Ln;
        L = L + Lt1 + Lt2;
        body1.setLinearVelocity(body1.linearVelocity() - L * body1.invMass());
        body1.setAngularVelocity(body1.angularVelocity() - body1.invInertia() * (r1 * L));

        body2.setLinearVelocity(body2.linearVelocity() + L * body2.invMass());
        body2.setAngularVelocity(body2.angularVelocity() + body2.invInertia() * (r2 * L));
    }
}

void ContactSolver::applyPseudoImpulse(float dt) {
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

bool ContactSolver::isCollide() {
    return contact_;
}

const ContactMainfold &ContactSolver::contactMainfold() {
    return contactMainfold_;
}
