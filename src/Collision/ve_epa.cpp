//
// Created by boris on 11/29/2021.
//

#include "ve_epa.h"
#include "ve_support_point.h"
#include "stdlibraries.h"
#include "ve_global_parameters.h"

using namespace VE;


EPA::Polytope::Polytope(const std::vector<Vector> &simplex) : vertices_(simplex),
                                                              faces_{Face(0, 1, 2),
                                                                     Face(1, 3, 2),
                                                                     Face(2, 3, 0),
                                                                     Face(0, 3, 1)} {
    updateFaceInfo();
}

void EPA::Polytope::updateFaceInfo() {
    for (size_t i = 0; i < faces_.size(); i++) {
        auto &face = faces_[i];
        const VE::Vector &A = vertices_[face.index[0]];
        const VE::Vector &B = vertices_[face.index[1]];
        const VE::Vector &C = vertices_[face.index[2]];

        VE::Vector BA = A - B;
        VE::Vector BC = C - B;
        face.normal = (BC * BA).normolize();
        face.distance = face.normal.dot(B);

        face.actualInfo = true;
    }
}

size_t EPA::Polytope::faceSize() const {
    return faces_.size();
}

size_t EPA::Polytope::verticesSize() const {
    return vertices_.size();
}

void EPA::Polytope::addFace(const EPA::Face &face) {
    faces_.emplace_back(face);
    updateFaceInfo();

#ifndef NDEBUG
    for (size_t i = 0; i < faces_.size(); i++) {
        assert(faces_[i].actualInfo);
    }
#endif
}

void EPA::Polytope::addVertex(const Vector &vertex) {
    vertices_.emplace_back(vertex);
}

void EPA::Polytope::deleteFace(size_t faceNumber) {
    faces_[faceNumber] = faces_.back();
    faces_.pop_back();
}

const EPA::Face &EPA::Polytope::closestFaceToOrigin() const {
    size_t minimalDistanceFaceNumber;
    float minimalDistance = std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < faces_.size(); i++) {
        assert(faces_[i].actualInfo && "Need uppdate face info befor using this function");
        if (faces_[i].distance < minimalDistance) {
            minimalDistance = faces_[i].distance;
            minimalDistanceFaceNumber = i;
        }
    }
    return faces_[minimalDistanceFaceNumber];
}

const EPA::Face &EPA::Polytope::operator[](size_t i) const {
    return faces_[i];
}

void EPA::Polytope::draw(VE::Vector a, const Color &color) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices_.data());

    for (int i = 0; i < faces_.size(); i++) {
        glColor3f(color.red() + i / 100.0, color.grean() + i / 100.0, color.blue() + i / 100.0);
        glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, faces_[i].index);
        Vector polygonCenter = (vertices_[faces_[i].index[0]] + vertices_[faces_[i].index[1]] + vertices_[faces_[i].index[2]]) / 3;
    }

    for (int i = 0; i < faces_.size(); i++) {
        Vector polygonCenter = (vertices_[faces_[i].index[0]] + vertices_[faces_[i].index[1]] + vertices_[faces_[i].index[2]]) / 3;
        if (faces_[i].normal.dot(a) > 0.0f) {
            faces_[i].normal.draw(polygonCenter, Color(1, 0, 0));
        } else {
            faces_[i].normal.draw(polygonCenter);
        }
    }


    vertices_[0].drawPoint(12, Color(1, 0, 0));
    vertices_[1].drawPoint(12, Color(0, 1, 0));
    vertices_[2].drawPoint(12, Color(0, 0, 1));
    for (size_t i = 3; i < vertices_.size(); i++) {
        vertices_[i].drawPoint(12, Color(1, 1, 1));
    }

    vertices_[3].drawPoint(12, Color(1, 1, 1));


    glDisableClientState(GL_VERTEX_ARRAY);
}

void EPA::Polytope::info(std::string s) {
    std::cout << s + " {" << std::endl;
    for (auto &face: faces_) {
        std::cout << "    " << face.index[0] << " " << face.index[1] << " " << face.index[2] << std::endl;
    }
    std::cout << "}" << std::endl;
}


void EPA::UnigueEdge::add(size_t a, size_t b) {
    auto reverse = std::find(
            edges_.begin(),
            edges_.end(),
            std::make_pair(b, a)
    );

    if (reverse != edges_.end()) {
        edges_.erase(reverse);
    } else {
        edges_.emplace_back(a, b);
    }
}

const std::vector<std::pair<size_t, size_t>> &EPA::UnigueEdge::edges() {
    return edges_;
}


EPA::EPA(const Collider &collider1, const Collider &collider2, const std::vector<Vector> &simplex) : collider1_(collider1),
                                                                                                     collider2_(collider2),
                                                                                                     polytope_(simplex) {

}

Vector EPA::getResolutionVector() {
    VE::Vector returnVector;

    for (int i = 0; i < globalParameters.epaIterations; i++) {
        const Face &closestFaceToOrigin = polytope_.closestFaceToOrigin();
        Vector extendVertex = getSupportPoint(collider1_, collider2_, closestFaceToOrigin.normal).point;
        float extendVertexDistance = extendVertex.dot(closestFaceToOrigin.normal);

        std::vector<float> direction;
        for (size_t i = 0; i < polytope_.faceSize(); i++) {
            direction.push_back(polytope_[i].normal.dot(extendVertex));
        }
        polytopeStage.push_back({polytope_, extendVertex, closestFaceToOrigin.normal, direction});

        if (abs(extendVertexDistance - closestFaceToOrigin.distance) > Collision::tolerance) {
            computeNewFaces(extendVertex);
        } else {
            returnVector = closestFaceToOrigin.normal * (closestFaceToOrigin.distance + Collision::tolerance);
            break;
        }
    }


    globalParameters.direction = polytopeStage[globalParameters.polytopeStage < polytopeStage.size() ? globalParameters.polytopeStage :
                                               polytopeStage.size() - 1].direction;
    globalParameters.minEpaNormal = polytopeStage[globalParameters.polytopeStage < polytopeStage.size() ? globalParameters.polytopeStage :
                                                  polytopeStage.size() - 1].minNormal;
    globalParameters.pointSize = polytope_.verticesSize();
    polytopeStage[globalParameters.polytopeStage < polytopeStage.size() ? globalParameters.polytopeStage : polytopeStage.size() - 1].polytope.draw(
            polytopeStage[globalParameters.polytopeStage < polytopeStage.size() ? globalParameters.polytopeStage : polytopeStage.size() -
                                                                                                                   1].support);
    polytopeStage[globalParameters.polytopeStage < polytopeStage.size() ? globalParameters.polytopeStage : polytopeStage.size() -
                                                                                                           1].support.draw(Color(0, 1, 0));
//    polytopeStage[globalParameters.polytopeStage < polytopeStage.size() ? globalParameters.polytopeStage : polytopeStage.size() - 1].minNormal.draw(
//            Color(1, 0, 0));
    return returnVector;
}

void EPA::computeNewFaces(const Vector &support) {
    UnigueEdge unigueEdge;
    for (size_t i = 0; i < polytope_.faceSize(); i++) {
        if (sameDirection(polytope_[i].normal, support)) {
            unigueEdge.add(polytope_[i].index[0], polytope_[i].index[1]);
            unigueEdge.add(polytope_[i].index[1], polytope_[i].index[2]);
            unigueEdge.add(polytope_[i].index[2], polytope_[i].index[0]);
            polytope_.deleteFace(i);
            i--;
        }
    }

    polytope_.addVertex(support);
    for (auto[i0, i1]: unigueEdge.edges()) {
        polytope_.addFace(Face(i0, i1, polytope_.verticesSize() - 1));
    }

}

bool EPA::sameDirection(const Vector &a, const Vector &b) {
    return a.dot(b) > 0.0f;
}