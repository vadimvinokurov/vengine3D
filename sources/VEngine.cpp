//
// Created by boris on 7/6/2022.
//

#include "VEngine.h"
#include "Window.h"
#include "RenderEngine.h"

VEngine::VEngine() {
	window_ = std::make_unique<Window>(1280, 720);
	window_->makeContextCurrent();
	renderEngine_ = std::make_unique<RenderEngine>();
	window_->OnWindowResizeDelegate.connect(renderEngine_.get(), &RenderEngine::resize);
	deltaTime_ = 1.0f / 60;
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
