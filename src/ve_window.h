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
#include "Control/ve_keyboard.hpp"
#include "Control/ve_mouse.hpp"

namespace VE {
    class Window final {
    public:
        explicit Window(int width = 1280, int height = 720, const std::string &label = "VEngine3D");
        void run();
        void setWorld(const WorldPtr &shownWorld);
        void setHid(const VE::KeyboardPtr &keyboard, const VE::MousePtr &mouse);
        ~Window();
    private:
        void guiInitialization();
        void windowInitialization(int width, int height, const std::string &label);
        static Window *getThis(GLFWwindow *window);

        void setCallbackFunction();

        void mouseButtonEvent(int button, int action, int mods);
        void cursorChangePositionEvent(const VE::Vector &cursorPosition);
        void keyEvent(int key, int scancode, int action, int mods);

        VE::Vector screeToOpenGLCoordinate(const VE::Vector &screenCoordinate);
        VE::Vector openGLToScreenCoordinate(const VE::Vector &openGLCoordinate);

        GLFWwindow *window_;
        float width_, height_;
        std::string label_;
        WorldPtr world_;
        float dt_ = 0;

        float windowAspectRatio_ = 1.0f;

        VE::MousePtr mouse_;
        VE::KeyboardPtr keyboard_;
    };
}


#endif //VENGINE3D_WINDOW_H
