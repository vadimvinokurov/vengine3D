//
// Created by boris on 7/6/2022.
//
#include "Window.h"

#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

Window::Window(int32 width, int32 height, const char *title)
{
	if (!glfwInit())
	{
		throw std::runtime_error("Glfw library is not initialized");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("glfw window is not initialized");
	}
	makeContextCurrent();
	glfwSwapInterval(0);
	glfwSetWindowUserPointer(window, this);
	glfwCallbackInitialization();
}

Window::~Window()
{
	glfwTerminate();
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(window);
}

std::pair<int32, int32> Window::getSize() const
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return {width, height};
}

float Window::getRation() const
{
	auto [width, height] = getSize();
	return width / static_cast<float>(height);
}

void Window::poolEvents()
{
	glfwPollEvents();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::swapBuffer()
{
	glfwSwapBuffers(window);
}
void Window::glfwCallbackInitialization()
{
	glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) {
		auto this_ptr = static_cast<Window *>(glfwGetWindowUserPointer(window));
		this_ptr->onWindowResizeDelegate.call(width, height);
	});

	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		auto this_ptr = static_cast<Window *>(glfwGetWindowUserPointer(window));
		this_ptr->onKeyboardKeyDelegate.call(key, action);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int key, int action, int mods) {
		auto this_ptr = static_cast<Window *>(glfwGetWindowUserPointer(window));
		this_ptr->onMouseKeyDelegate.call(key, action);

		if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			this_ptr->mouseLockFlag = true;
			glfwGetCursorPos(window, &this_ptr->lockXpos, &this_ptr->lockYpos);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			this_ptr->mouseLockFlag = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
		auto this_ptr = static_cast<Window *>(glfwGetWindowUserPointer(window));
		this_ptr->onMousePositionDelegate.call(static_cast<float>(xpos), static_cast<float>(ypos));
		if(this_ptr->mouseLockFlag) {
			double cxpos, cypos;
			glfwGetCursorPos(window, &cxpos, &cypos);
			this_ptr->onMouseDeltaPositionDelegate.call(cxpos - this_ptr->lockXpos, cypos - this_ptr->lockYpos);
			glfwSetCursorPos(window, this_ptr->lockXpos, this_ptr->lockYpos);
		}

	});
}
void Window::setTitle(const std::string &title)
{
	glfwSetWindowTitle(window, title.c_str());
}
