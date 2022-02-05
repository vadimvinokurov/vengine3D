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
    VE::Vector supportPoint = getSupportPoint(collider1_, collider2_, direction).point;

    simplex.push_back(supportPoint);
    direction = (supportPoint * -1).normalized();
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
    const VE::Vector &A = simplex[0];
    const VE::Vector &B = simplex[1];
    VE::Vector BA = A - B;
    VE::Vector BO = B * -1;

    if (sameDirection(BA * -1, BO)) {
        simplex = {B};
        direction = BO;
    } else {
        direction = BA * BO * BA;
    }
    return false;
}

bool GJK::triangleCase() {

    VE::Vector &A = simplex[0];
    VE::Vector &B = simplex[1];
    VE::Vector &C = simplex[2];

    VE::Vector CB = B - C;
    VE::Vector CA = A - C;
    VE::Vector CO = C * -1;

    VE::Vector BCA = CB * CA;
    VE::Vector CAn = BCA * CA;
    VE::Vector CBn = CB * BCA;

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
    VE::Vector &A = simplex[0];
    VE::Vector &B = simplex[1];
    VE::Vector &C = simplex[2];
    VE::Vector &D = simplex[3];

    VE::Vector DA = A - D;
    VE::Vector DB = B - D;
    VE::Vector DC = C - D;
    VE::Vector DO = D * -1;

    VE::Vector BDA = DB * DA;
    VE::Vector CDB = DC * DB;
    VE::Vector ADC = DA * DC;

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

const std::vector<VE::Vector> &GJK::getSimplex() const {
    return simplex;
}
