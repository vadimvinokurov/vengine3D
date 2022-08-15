//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_INPUTCOMPONENTS_H
#define VENGINE3D_INPUTCOMPONENTS_H

#include "Core/Objects/Component.h"

class InputComponents : public Component<InputComponents>
{
public:
	void press(){
		spdlog::info("pess {}", this->getOwner());
	}
};

#endif // VENGINE3D_INPUTCOMPONENTS_H
