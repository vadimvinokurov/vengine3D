//
// Created by boris on 12/3/2021.
//
#include "ve_epa_polytope.h"

using namespace VE;

EPA::Polytope::Polytope(const std::vector<Vector> &simplex) : vertices_(simplex),
                                                              faces_{Face(0, 1, 2),
                                                                     Face(1, 3, 2),
                                                                     Face(2, 3, 0),
                                                                     Face(0, 3, 1)} {
    updateFacesData();
}

void EPA::Polytope::addFace(const EPA::Face &face) {
    faces_.emplace_back(face);
    updateFacesData();
}

void EPA::Polytope::deleteFace(size_t faceNumber) {
    assert(faceNumber < faces_.size() && "Face number for delete is not correct.");
    assert(faces_.size() != 0 && "Polytope have not faces.");
    faces_[faceNumber] = faces_.back();
    faces_.pop_back();
}

size_t EPA::Polytope::getFacesSize() const {
    return faces_.size();
}

const EPA::Face &EPA::Polytope::operator[](size_t faceNumber) const {
    return faces_[faceNumber];
}

void EPA::Polytope::addVertex(const Vector &vertex) {
    vertices_.emplace_back(vertex);
}

const Vector &EPA::Polytope::getFaceVertex(size_t faceNumber, size_t vertexNumber) const {
    return vertices_[faces_[faceNumber].index[vertexNumber]];
}

size_t EPA::Polytope::getVerticesSize() const {
    return vertices_.size();
}

const EPA::Face &EPA::Polytope::getClosestFaceToOrigin() const {
    size_t minimalDistanceFaceNumber;
    float minimalDistance = std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < faces_.size(); i++) {
        assert(faces_[i].actualData && "Need uppdate face info befor using this function");
        if (faces_[i].distance < minimalDistance) {
            minimalDistance = faces_[i].distance;
            minimalDistanceFaceNumber = i;
        }
    }
    return faces_[minimalDistanceFaceNumber];
}

void EPA::Polytope::updateFacesData() {
    for (size_t i = 0; i < faces_.size(); i++) {
        auto &face = faces_[i];
        const VE::Vector &A = vertices_[face.index[0]];
        const VE::Vector &B = vertices_[face.index[1]];
        const VE::Vector &C = vertices_[face.index[2]];

        VE::Vector BA = A - B;
        VE::Vector BC = C - B;
        face.normal = (BC * BA).getNormalized();
        face.distance = face.normal.dot(B);

        face.actualData = true;
    }
}

void EPA::Polytope::debugDraw(const Color &color) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices_.data());

    for (int i = 0; i < faces_.size(); i++) {
        glColor3f(color.red() + i / 100.0, color.grean() + i / 100.0, color.blue() + i / 100.0);
        glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, faces_[i].index);
    }

    for (int i = 0; i < faces_.size(); i++) {
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

