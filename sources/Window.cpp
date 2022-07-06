//
// Created by boris on 7/6/2022.
//
#include <stdexcept>
#include "Window.h"

using namespace VECore;

Window::Window(int32 width, int32 height, const char* title) {
	if (!glfwInit()) {
		throw std::runtime_error("Glfw library is not initialized");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("glfw window is not initialized");
	}
	makeContextCurrent();
}

Window::~Window() {
	glfwTerminate();
}

void Window::makeContextCurrent() {
	glfwMakeContextCurrent(window);
}

std::pair<int32, int32> Window::getSize() const {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return {width, height};
}

float32 Window::getRation() const {
	auto [width, height] = getSize();
	return width / static_cast<float32>(height);
}

void Window::poolEvents() {
	glfwPollEvents();
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(window);
}

void Window::swapBuffer() {
	glfwSwapBuffers(window);
}
