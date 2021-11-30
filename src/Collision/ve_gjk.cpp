//
// Created by boris on 11/28/2021.
//

#include "ve_gjk.h"
#include "ve_epa.h"
#include "ve_collision_common.h"
#include "ve_support_point.h"

using namespace VE;


GJK::GJK(const Collider &collider1, const Collider &collider2) : collider1_(collider1),
                                                                 collider2_(collider2) {
}

bool GJK::testIntersection(Vector &penetrationVector) {
    VE::Vector supportPoint = getSupportPoint(collider1_, collider2_, direction).point;

    simplex.push_back(supportPoint);
    direction = (supportPoint * -1).normolize();
    while (1) {
        supportPoint = getSupportPoint(collider1_, collider2_, direction).point;
        if (supportPoint.dot(direction) <= 0) return false;
        simplex.push_back(supportPoint);

        if (nextSimplex()) {
            penetrationVector = EPA(collider1_, collider2_, simplex).getResolutionVector();
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
            simplex = {C, A};
            direction = CA * CO * CA;
        } else {
            if (sameDirection(CB, CO)) {
                simplex = {C, B};
                direction = CB * CO * CB;
            } else {
                simplex = {C};
                direction = CO;
            }
        }
    } else {
        if (sameDirection(CBn, CO)) {
            if (sameDirection(CB, CO)) {
                simplex = {C, B};
                direction = CB * CO * CB;
            } else {
                simplex = {C};
                direction = CO;
            }
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

    VE::Vector ADB = DA * DB;
    VE::Vector BDC = DB * DC;
    VE::Vector CDA = DC * DA;

    VE::Vector CA = A - C;
    VE::Vector CB = B - C;
    VE::Vector CO = C * -1;
    VE::Vector ACD = CA * CB;

    if (sameDirection(ADB, DO)) {
        simplex = {A, B, D};
        direction = ADB;

        return false;
    } else if (sameDirection(BDC, DO)) {
        simplex = {B, C, D};
        direction = BDC;

        return false;
    } else if (sameDirection(CDA, DO)) {
        simplex = {C, A, D};
        direction = CDA;

        return false;
    } else {
        return true;
    }
}

bool GJK::sameDirection(const Vector &a, const Vector &b) {
    return a.dot(b) > 0.0f;
}

bool GJK::perpendicularVector(const Vector &a, const Vector &b) {
    return a.dot(b) == 0.0f;
}

