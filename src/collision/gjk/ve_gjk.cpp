//
// Created by boris on 11/28/2021.
//

#include "ve_gjk.h"
#include "collision/epa/ve_epa.h"
#include "collision/ve_collision_common.h"
#include "collision/ve_support_point.h"

using namespace VE;


GJK::GJK(const Collider &collider1, const Collider &collider2) : collider1_(collider1),
                                                                 collider2_(collider2) {
}

bool GJK::testIntersection() {
    VE::Vector3 supportPoint = getSupportPoint(collider1_, collider2_, direction).point;

    simplex.push_back(supportPoint);
    direction = (supportPoint * -1).getNormalized();
    while (1) {
        supportPoint = getSupportPoint(collider1_, collider2_, direction).point;
        if (supportPoint.dot(direction) <= 0) {
            return false;
        }
        simplex.push_back(supportPoint);

        if (nextSimplex()) {
            return true;
        }
    }
}

bool GJK::nextSimplex() {
    switch (simplex.size()) {
        case 2:
            return lineCase();
        case 3:
            return triangleCase();
        case 4:
            return tetrahedronCase();
        default:
            return false;
    }
}

bool GJK::lineCase() {
    const VE::Vector3 &A = simplex[0];
    const VE::Vector3 &B = simplex[1];
    VE::Vector3 BA = A - B;
    VE::Vector3 BO = B * -1;

    if (sameDirection(BA * -1, BO)) {
        simplex = {B};
        direction = BO;
    } else {
        direction = BA * BO * BA;
    }
    return false;
}

bool GJK::triangleCase() {

    VE::Vector3 &A = simplex[0];
    VE::Vector3 &B = simplex[1];
    VE::Vector3 &C = simplex[2];

    VE::Vector3 CB = B - C;
    VE::Vector3 CA = A - C;
    VE::Vector3 CO = C * -1;

    VE::Vector3 BCA = CB * CA;
    VE::Vector3 CAn = BCA * CA;
    VE::Vector3 CBn = CB * BCA;

    if (sameDirection(CAn, CO)) {
        if (sameDirection(CA, CO)) {
            simplex = {A, C};
            direction = CA * CO * CA;
        } else {
            simplex = {B, C};
            return lineCase();
        }
    } else {
        if (sameDirection(CBn, CO)) {
            simplex = {B, C};
            return lineCase();
        } else {
            if (sameDirection(BCA, CO)) {
                direction = BCA;
            } else {
                simplex = {B, A, C};
                direction = BCA * -1;
            }
        }
    }
    return false;
}

bool GJK::tetrahedronCase() {
    VE::Vector3 &A = simplex[0];
    VE::Vector3 &B = simplex[1];
    VE::Vector3 &C = simplex[2];
    VE::Vector3 &D = simplex[3];

    VE::Vector3 DA = A - D;
    VE::Vector3 DB = B - D;
    VE::Vector3 DC = C - D;
    VE::Vector3 DO = D * -1;

    VE::Vector3 BDA = DB * DA;
    VE::Vector3 CDB = DC * DB;
    VE::Vector3 ADC = DA * DC;

    if (sameDirection(BDA, DO)) {
        simplex = {A, B, D};
        return triangleCase();
    } else if (sameDirection(CDB, DO)) {
        simplex = {B, C, D};
        return triangleCase();
    } else if (sameDirection(ADC, DO)) {
        simplex = {C, A, D};
        return triangleCase();
    } else {
        return true;
    }
}

const std::vector<VE::Vector3> &GJK::getSimplex() const {
    return simplex;
}
