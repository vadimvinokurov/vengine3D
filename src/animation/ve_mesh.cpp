//
// Created by boris on 2/20/2022.
//

#include "ve_mesh.h"
using namespace VE;

Mesh::Mesh(const Mesh& other) {}

Mesh& Mesh::operator=(const Mesh& other) {
    return *this;
}

void Mesh::updateOpenGLBuffers() {
    positionsGPU.set(positions);
    normalsGPU.set(normals);
    textureCoordinatesGPU.set(textureCoordinates);
    jointsInfluencesWeightsGPU.set(jointsInfluencesWeights);
    jointsInfluencesGPU.set(jointsInfluences);
    indicesGPU.set(indices);
}

void Mesh::skin(const Skeleton& skeleton, const Pose& pose) {}
