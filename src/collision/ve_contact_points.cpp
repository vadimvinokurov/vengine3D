//
// Created by boris on 12/7/2021.
//

#include "ve_contact_points.h"
#include "ve_global_parameters.h"

using namespace VE;

ContactPoint::ContactPoint(const BoxCollider &collider1, const BoxCollider &collider2, const Vector &contactNormal) {
    ColliderFace faceC1 = collider1.getFaceInDirection(contactNormal);
    ColliderFace faceC2 = collider2.getFaceInDirection(contactNormal * -1);

    bool isFaceC1Reference = selectReferenceEdge(faceC1, faceC2, contactNormal);
    if (isFaceC1Reference) {
        generateClipPlanes(faceC1);
        doubleVertexBuffer.buffer2() = faceC2.vertices;
    } else {
        generateClipPlanes(faceC2);
        doubleVertexBuffer.buffer2() = faceC1.vertices;
    }

}

bool ContactPoint::selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2, const VE::Vector& contactNormal) {
    return abs(face1.normal.dot(contactNormal)) > abs(face2.normal.dot(contactNormal));
}

std::vector<VE::Vector> ContactPoint::get() {
    for (const ClipPlane &clipPlane: clipPlanes) {
        doubleVertexBuffer.swapBuffer();
        const std::vector<VE::Vector> &inputVertices = doubleVertexBuffer.buffer1();
        std::vector<VE::Vector> &outputVertices = doubleVertexBuffer.buffer2();
        outputVertices.clear();

        for (size_t i = 0; i < inputVertices.size(); i++) {
            const VE::Vector &B = inputVertices[i];
            const VE::Vector &A = inputVertices[i == 0 ? inputVertices.size() - 1 : i - 1];

            if (vertexInsidePlane(B, clipPlane)) {
                if (!vertexInsidePlane(A, clipPlane)) {
                    outputVertices.emplace_back(intersectionPoint(A, B, clipPlane));
                }
                outputVertices.emplace_back(B);
            } else if (vertexInsidePlane(A, clipPlane)) {
                outputVertices.emplace_back(intersectionPoint(A, B, clipPlane));
            }
        }
    }

    deleteVertexOutsideMainFace(doubleVertexBuffer.buffer2());
    return doubleVertexBuffer.buffer2();
}

void ContactPoint::generateClipPlanes(const ColliderFace &referenceFace) {
    for (unsigned int i = 0; i < referenceFace.vertices.size() - 1; i++) {
        VE::Vector planeNormal = (referenceFace.vertices[i + 1] - referenceFace.vertices[i]) * referenceFace.normal;
        VE::Vector planeVertex = referenceFace.vertices[i];
        clipPlanes.emplace_back(planeNormal, planeVertex);
    }
    VE::Vector planeNormal = (referenceFace.vertices.front() - referenceFace.vertices.back()) * referenceFace.normal;
    VE::Vector planeVertex = referenceFace.vertices.back();
    clipPlanes.emplace_back(planeNormal, planeVertex);

    mainPlane = ClipPlane(referenceFace.normal, referenceFace.vertices.front());
}

VE::Vector ContactPoint::intersectionPoint(const VE::Vector &A, const VE::Vector &B, const ClipPlane &clipPlane) {
    const Vector &n = clipPlane.normal;
    const Vector &P = clipPlane.point;
    float d = clipPlane.d;
    float t = (d - A.dot(n)) / ((B - A).dot(n));
    return A + (B - A) * t;
}

bool ContactPoint::vertexInsidePlane(const Vector &A, const ContactPoint::ClipPlane &clipPlane) {
    return (A - clipPlane.point).dot(clipPlane.normal) <= 0;
}

void ContactPoint::deleteVertexOutsideMainFace(std::vector<VE::Vector> &vertices) {
    for (size_t i = 0; i < vertices.size(); i++) {
        if (!vertexInsidePlane(vertices[i], mainPlane)) {
            vertices[i] = vertices.back();
            vertices.pop_back();
            i--;
        }
    }
}
