//
// Created by boris on 12/7/2021.
//

#include "ve_contact_points.h"
#include "ve_global_parameters.h"

using namespace VE;

BoxBoxContactPoint::BoxBoxContactPoint(const BoxCollider &collider1, const BoxCollider &collider2, const Vector3 &contactNormal) {
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

bool BoxBoxContactPoint::selectReferenceEdge(const ColliderFace &face1, const ColliderFace &face2, const VE::Vector3& contactNormal) {
    return abs(face1.normal.dot(contactNormal)) > abs(face2.normal.dot(contactNormal));
}

std::vector<VE::Vector3> BoxBoxContactPoint::get() {
    for (const ClipPlane &clipPlane: clipPlanes) {
        doubleVertexBuffer.swapBuffer();
        const std::vector<VE::Vector3> &inputVertices = doubleVertexBuffer.buffer1();
        std::vector<VE::Vector3> &outputVertices = doubleVertexBuffer.buffer2();
        outputVertices.clear();

        for (size_t i = 0; i < inputVertices.size(); i++) {
            const VE::Vector3 &B = inputVertices[i];
            const VE::Vector3 &A = inputVertices[i == 0 ? inputVertices.size() - 1 : i - 1];

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

void BoxBoxContactPoint::generateClipPlanes(const ColliderFace &referenceFace) {
    for (unsigned int i = 0; i < referenceFace.vertices.size() - 1; i++) {
        VE::Vector3 planeNormal = (referenceFace.vertices[i + 1] - referenceFace.vertices[i]) * referenceFace.normal;
        VE::Vector3 planeVertex = referenceFace.vertices[i];
        clipPlanes.emplace_back(planeNormal, planeVertex);
    }
    VE::Vector3 planeNormal = (referenceFace.vertices.front() - referenceFace.vertices.back()) * referenceFace.normal;
    VE::Vector3 planeVertex = referenceFace.vertices.back();
    clipPlanes.emplace_back(planeNormal, planeVertex);

    mainPlane = ClipPlane(referenceFace.normal, referenceFace.vertices.front());
}

VE::Vector3 BoxBoxContactPoint::intersectionPoint(const VE::Vector3 &A, const VE::Vector3 &B, const ClipPlane &clipPlane) {
    const Vector3 &n = clipPlane.normal;
    const Vector3 &P = clipPlane.point;
    float d = clipPlane.d;
    float t = (d - A.dot(n)) / ((B - A).dot(n));
    return A + (B - A) * t;
}

bool BoxBoxContactPoint::vertexInsidePlane(const Vector3 &A, const BoxBoxContactPoint::ClipPlane &clipPlane) {
    return (A - clipPlane.point).dot(clipPlane.normal) <= 0;
}

void BoxBoxContactPoint::deleteVertexOutsideMainFace(std::vector<VE::Vector3> &vertices) {
    for (size_t i = 0; i < vertices.size(); i++) {
        if (!vertexInsidePlane(vertices[i], mainPlane)) {
            vertices[i] = vertices.back();
            vertices.pop_back();
            i--;
        }
    }
}
