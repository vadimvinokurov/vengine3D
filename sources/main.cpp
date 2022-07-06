//
// Created by boris on 7/6/2022.
//
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	printf("OpenGL version supported by this platform (%s): \n",
		   glGetString(GL_VERSION));
	std::cout << "Hello World" << std::endl;
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
	}
	return 0;
}