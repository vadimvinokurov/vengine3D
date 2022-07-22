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
		this_ptr->OnWindowResizeDelegate.call(width, height);
	});
}
void Window::setTitle(const std::string &title)
{
	glfwSetWindowTitle(window, title.c_str());
}
