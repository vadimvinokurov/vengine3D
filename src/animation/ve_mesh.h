//
// Created by boris on 2/20/2022.
//

#ifndef VENGINE3D_SRC_ANIMATION_VE_MESH_H_
#define VENGINE3D_SRC_ANIMATION_VE_MESH_H_
#include "ve_core.h"
#include "math/ve_vector.h"
#include "math/ve_matrix4.h"
#include "render/ve_attribute.h"
#include "render/ve_index_buffer.h"
#include "ve_skeleton.h"
namespace VE {
    class Mesh {
    public:
        Mesh() = default;

        ~Mesh() = default;

        Mesh(const Mesh& other);

        Mesh& operator=(const Mesh& other);

        void skin(const Skeleton& skeleton);

        void updateOpenGLBuffers();

        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Vector2> textureCoordinates;
        std::vector<Vector4> jointsInfluencesWeights;
        std::vector<IVector4> jointsInfluences;
        std::vector<uint32> indices;

        Attribute<Vector3> positionsGPU;
        Attribute<Vector3> normalsGPU;
        Attribute<Vector2> textureCoordinatesGPU;
        Attribute<Vector4> jointsInfluencesWeightsGPU;
        Attribute<IVector4> jointsInfluencesGPU;
        IndexBuffer indicesGPU;

    private:
        std::vector<Vector3> skinnedPosition_;
        std::vector<Vector3> skinnedNormals_;
        std::vector<Matrix4> posePalette_;
    };
}  // namespace VE


#endif  //VENGINE3D_SRC_ANIMATION_VE_MESH_H_
