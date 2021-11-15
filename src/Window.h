//
// Created by boris on 11/13/2021.
//

#ifndef VENGINE3D_WINDOW_H
#define VENGINE3D_WINDOW_H

#include <GL/gl.h>
#include <string>
#include <iostream>

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "World.h"

namespace VE {
    class Window final {
    public:
        explicit Window(int width = 1280, int height = 720, const std::string &label = "VEngine3D");
        void run();
        void setWorld(WorldPtr shownWorld);
        ~Window();
    private:
        void guiInitialization();
        void windowInitialization(int width, int height, const std::string &label);
        static Window *getThis(GLFWwindow *window);

        GLFWwindow *window_;
        std::string label_;
        WorldPtr shownWorld_;
        float dt_ = 0;
    };
}


#endif //VENGINE3D_WINDOW_H
