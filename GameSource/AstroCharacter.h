//
// Created by boris on 20.09.2022.
//

#ifndef VENGINE3D_ASTROCHARACTER_H
#define VENGINE3D_ASTROCHARACTER_H

#include "BaseCharacter.h"
#include "AssetImporter.h"
class AstroCharacter : public BaseCharacter
{
	GENERATE_ENTITY_BODY(BaseCharacter)
public:
	void onCreate() override
	{
		Super::onCreate();
		std::vector<ShaderSource> shaderSource;
		shaderSource.emplace_back("../Content/Shaders/default.frag", ShaderType::Fragment);
		shaderSource.emplace_back("../Content/Shaders/animdefault.vert", ShaderType::Vertex);

		AssetImporter astroAsset = AssetImporter("../Content/Mesh/astroboy/astroBoy_walk_Max.dae");

		auto skeletalMeshModel = std::make_shared<SkeletalMeshModel>(astroAsset.getSkeletalMesh());
		skeletalMeshComponent->skeletalMesh.setSkeletalMeshModel(skeletalMeshModel);

		skeletalMeshComponent->skeletalMesh.materials[0] =
			std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[1] =
			std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[2] =
			std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);
		skeletalMeshComponent->skeletalMesh.materials[3] =
			std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);

		Skeleton restPose = astroAsset.getSkeleton();
		auto animations = astroAsset.getAnimations();

		skeletalMeshComponent->skeleton = std::make_shared<Skeleton>(restPose);

		skeletalMeshComponent->animation = std::make_shared<Animation>(animations.begin()->second);
		skeletalMeshComponent->animation->setLooping(true);
	}
};

#endif // VENGINE3D_ASTROCHARACTER_H
