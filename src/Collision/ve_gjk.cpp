//
// Created by boris on 11/28/2021.
//

#include "ve_gjk.h"
#include "ve_epa.h"
#include "ve_collision_common.h"
#include "ve_support_point.h"

using namespace VE;

bool lineCase(std::vector<VE::Vector> &simplex, VE::Vector &direction) {
    const VE::Vector &A = simplex[0];
    const VE::Vector &B = simplex[1];
    VE::Vector BA = A - B;
    VE::Vector BO = B * -1;
    direction = ((BA * BO) * BA).normolize();

    return false;
}

bool triangleCase(std::vector<VE::Vector> &simplex, VE::Vector &direction) {

    VE::Vector &A = simplex[0];
    VE::Vector &B = simplex[1];
    VE::Vector &C = simplex[2];

    VE::Vector CB = B - C;
    VE::Vector CA = A - C;
    VE::Vector CO = C * -1;

    VE::Vector BCA = (CB * CA).normolize();

    if (BCA.dot(CO) > 0) {
        direction = BCA;
    } else {
        simplex = {B, A, C};
        direction = BCA * -1;
    }
    return false;
}

bool tetrahedronCase(std::vector<VE::Vector> &simplex, VE::Vector &direction) {
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

    if (ADB.dot(DO) >= 0) {
        simplex = {A, B, D};
        direction = ADB.normolize();

        return false;
    } else if (BDC.dot(DO) >= 0) {
        simplex = {B, C, D};
        direction = BDC.normolize();

        return false;
    } else if (CDA.dot(DO) >= 0) {
        simplex = {C, A, D};
        direction = CDA.normolize();

        return false;
    } else {
        return true;
    }
}

void drawSimplex(std::vector<VE::Vector> &simplex) {

    std::vector<unsigned int> index = {0, 1, 2,
                                       1, 2, 3,
                                       2, 0, 3,
                                       0, 1, 3
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, simplex.data());

    for (int i = 0; i < index.size() / 3; i++) {
        glColor3f(0.50f + i / 100.0, 0.50f + i / 100.0, 0.50f + i / 100.0);
        glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, index.data() + i * 3);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

bool nextSimplex(std::vector<VE::Vector> &simplex, VE::Vector &direction) {
    switch (simplex.size()) {
        case 2:
            return lineCase(simplex, direction);
        case 3:
            return triangleCase(simplex, direction);
        case 4:
            return tetrahedronCase(simplex, direction);
        default:
            return false;
    }
}

bool VE::gjk(const Collider &collider1, const Collider &collider2, VE::Vector &penetrationVector) {
    std::vector<VE::Vector> simplex;
    VE::Vector direction = collider1.farthestVertexInDirection(Vector(0, 1, 0)) * -1;
    VE::Vector supportPoint = getSupportPoint(collider1, collider2, direction).point;

    simplex.push_back(supportPoint);
    direction = (supportPoint * -1).normolize();
    while (1) {
        supportPoint = getSupportPoint(collider1, collider2, direction).point;
        if (supportPoint.dot(direction) <= 0) return false;
        simplex.push_back(supportPoint);

        if (nextSimplex(simplex, direction)) {
            penetrationVector = EPA(collider1, collider2, simplex).getResolutionVector();
            penetrationVector.draw(Vector(1, 1, 1));
            std::cout << "-------------" << std::endl;
            return true;
        }
    }
}