//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_VENGINE_H
#define VENGINE3D_VENGINE_H

#include "EngineCore.h"

class RenderEngine;
class Window;

class VEngine {
public:
	VEngine();
	~VEngine();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onQuite();

	void run();

protected:
	std::unique_ptr<Window> window_;
	std::unique_ptr<RenderEngine> renderEngine_;
	float32 deltaTime_;
};


#endif	//VENGINE3D_VENGINE_H
