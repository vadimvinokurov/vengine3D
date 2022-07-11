//
// Created by boris on 7/6/2022.
//

#include "VEngine.h"
#include "Window.h"
#include "Rendering/RenderEngine.h"

VEngine::VEngine() {
	window_ = CreateUniqueObject<Window>(windowDefaultWidth_, windowDefaultHeight_);
	window_->makeContextCurrent();
	renderEngine_ = CreateUniqueObject<RenderEngine>();
	renderEngine_->resize(windowDefaultWidth_, windowDefaultHeight_);
	window_->OnWindowResizeDelegate.connect(renderEngine_.get(), &RenderEngine::resize);
}

void VEngine::run() {
	onCreate();

	while (!window_->shouldClose()) {
		auto frameStart = std::chrono::steady_clock::now();
		Window::poolEvents();
		onUpdate();
		window_->swapBuffer();

		std::chrono::duration<double> target(deltaTime_);
		while (std::chrono::steady_clock::now() - frameStart < target) {
			std::this_thread::yield();
		}
	}
	onQuite();
}
VEngine::~VEngine() {}

void VEngine::onCreate() {}

void VEngine::onUpdate() {
	renderEngine_->clear();
}

void VEngine::onQuite() {}
