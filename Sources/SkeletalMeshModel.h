//
// Created by boris on 8/30/2022.
//

#ifndef VENGINE3D_SKELETALMESHMODEL_H
#define VENGINE3D_SKELETALMESHMODEL_H

#include "SkeletalMeshSection.h"
#include "SkeletalMeshSectionRenderData.h"
#include "Render/VAO.h"

class SkeletalMeshModel
{
public:
	void addSkeletalMeshSection(const SkeletalMeshSection &section)
	{
		sections.push_back(section);
		renderData.emplace_back(section);
		VAOs.emplace_back();

		VAOs.back().bind();
		renderData.back().use();
		VAOs.back().unbind();
	}

	uint32 getSectionCount()
	{
		return sections.size();
	}

	std::pair<const VAO &, uint32> getRenderData(uint32 index) const
	{
		return {VAOs[index], renderData[index].indices.count()};
	}

private:
	std::vector<SkeletalMeshSection> sections;
	std::vector<SkeletalMeshSectionRenderData> renderData;
	std::vector<VAO> VAOs;
};

#endif // VENGINE3D_SKELETALMESHMODEL_H
