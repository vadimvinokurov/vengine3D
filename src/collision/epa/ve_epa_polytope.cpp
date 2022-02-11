//
// Created by boris on 12/3/2021.
//
#include "ve_epa_polytope.h"

using namespace VE;

EPA::Polytope::Polytope(const std::vector<Vector3> &simplex) : vertices_(simplex),
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

void EPA::Polytope::addVertex(const Vector3 &vertex) {
    vertices_.emplace_back(vertex);
}

const Vector3 &EPA::Polytope::getFaceVertex(size_t faceNumber, size_t vertexNumber) const {
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
        const VE::Vector3 &A = vertices_[face.index[0]];
        const VE::Vector3 &B = vertices_[face.index[1]];
        const VE::Vector3 &C = vertices_[face.index[2]];

        VE::Vector3 BA = A - B;
        VE::Vector3 BC = C - B;
        face.normal = (BC * BA).getNormalized();
        face.distance = face.normal.dot(B);

        face.actualData = true;
    }
}

