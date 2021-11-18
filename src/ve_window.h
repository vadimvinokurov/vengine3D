//
// Created by boris on 11/13/2021.
//

#ifndef VENGINE3D_WINDOW_H
#define VENGINE3D_WINDOW_H

#include <string>
#include <iostream>

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ve_world.h"

namespace VE {
    class Window final {
    public:
        explicit Window(int width = 1280, int height = 720, const std::string &label = "VEngine3D");
        void run();
        void setWorld(const WorldPtr& shownWorld);
        ~Window();
    private:
        void guiInitialization();
        void windowInitialization(int width, int height, const std::string &label);
        static Window *getThis(GLFWwindow *window);

        GLFWwindow *window_;
        std::string label_;
        WorldPtr shownWorld_;
        float dt_ = 0;

        float windowAspectRatio_ = 1.0f;
    };
}


#endif //VENGINE3D_WINDOW_H
