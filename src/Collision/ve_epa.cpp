//
// Created by boris on 11/29/2021.
//

#include "ve_epa.h"
#include "ve_support_point.h"
#include "stdlibraries.h"


using namespace VE;


EPA::Polytope::Polytope(const std::vector<Vector> &simplex) : vertices_(simplex),
                                                              faces_{Face(0, 1, 2),
                                                                     Face(1, 3, 2),
                                                                     Face(2, 3, 0),
                                                                     Face(0, 3, 1) } {
    updateFaceInfo();
}

void EPA::Polytope::updateFaceInfo() {
    for (size_t i = 0; i < faces_.size(); i++) {
        auto &face = faces_[i];
        const VE::Vector &A = vertices_[face.index[0]];
        const VE::Vector &B = vertices_[face.index[1]];
        const VE::Vector &C = vertices_[face.index[2]];

        VE::Vector BA = B - A;
        VE::Vector BC = C - A;
        face.normal = (BA * BC).normolize();
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

size_t EPA::Polytope::minimalDistanceFace() const {
    size_t minimalDistanceFaceNumber;
    float minimalDistance = std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < faces_.size(); i++) {
        assert(faces_[i].actualInfo && "Need uppdate face info befor using this function");
        if (faces_[i].distance < minimalDistance) {
            minimalDistance = faces_[i].distance;
            minimalDistanceFaceNumber = i;
        }
    }
    return minimalDistanceFaceNumber;
}

const EPA::Face &EPA::Polytope::operator[](size_t i) const {
    return faces_[i];
}

void EPA::Polytope::draw(const Color &color) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices_.data());

    for (int i = 0; i < faces_.size(); i++) {
        glColor3f(color.red() + i / 100.0, color.grean() + i / 100.0, color.blue() + i / 100.0);
        glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, faces_[i].index);
        Vector polygonCenter = (vertices_[faces_[i].index[0]] + vertices_[faces_[i].index[1]] + vertices_[faces_[i].index[2]]) / 3;
        faces_[i].normal.draw(polygonCenter);
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
            std::make_pair(a, b)
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
    float minimalDistance;
    Vector minimalNormal;
    for (int i = 0; i < 1; i++) {
        size_t minimalDistanceFace = polytope_.minimalDistanceFace();
        minimalNormal = polytope_[minimalDistanceFace].normal;
        minimalDistance = polytope_[minimalDistanceFace].distance;

        Vector support = getSupportPoint(collider2_, collider2_, minimalNormal).point;
        float supportDistance = support.dot(minimalNormal);
        if (abs(supportDistance - minimalDistance) > Collision::tolerance) {
            support.drawPoint(12);
            //computeNewFaces(support);
        } else {
            break;
        }
    }
    polytope_.draw();
    return minimalNormal * (minimalDistance + Collision::tolerance);
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
