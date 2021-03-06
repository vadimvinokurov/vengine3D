//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_FRAME_H
#define VENGINE3D_VE_FRAME_H

#include "ve_core.h"

namespace VE {
	template<typename T>
	struct Frame {
		T value;
		T in;
		T out;
		float time;
	};
}  // namespace VE

#endif	//VENGINE3D_VE_FRAME_H
