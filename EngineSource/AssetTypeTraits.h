//
// Created by boris on 9/2/2022.
//

#ifndef VENGINE3D_ASSETTYPETRAITS_H
#define VENGINE3D_ASSETTYPETRAITS_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Math/Vector.h"
#include "Math/Quat.h"
#include "AnimFrame.h"

template <typename T>
struct AssetSameType
{
	using type = void;
};

template <>
struct AssetSameType<aiQuatKey>
{
	using type = AnimFrame<Quaternion>;
};

template <>
struct AssetSameType<aiVectorKey>
{
	using type = AnimFrame<Vector3>;
};

template <>
struct AssetSameType<aiQuaternion>
{
	using type = Quaternion;
};

template <>
struct AssetSameType<aiVector3D>
{
	using type = Vector3;
};

template <typename T>
using AssetSameType_t = typename AssetSameType<T>::type;

#endif // VENGINE3D_ASSETTYPETRAITS_H
