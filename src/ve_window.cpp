//
// Created by boris on 11/13/2021.
//

#include "ve_window.h"
#include "ve_render.h"

#include <stdexcept>
#include <cmath>

using namespace VE;

Window::Window(int width, int height, const std::string &label) : label_(label) {
    windowInitialization(width, height, label);
    guiInitialization();
}

void Window::windowInitialization(int width, int height, const std::string &label) {
    glfwInit();
    window_ = glfwCreateWindow(width, height, label.c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }
    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, this);
}

void Window::guiInitialization() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::StyleColorsDark();
}

Window *Window::getThis(GLFWwindow *window) {
    return static_cast<Window *>(glfwGetWindowUserPointer(window));
}

Window::~Window() {
    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Window::run() {
    if (shownWorld_ == nullptr) {
        std::cerr << "Window hasn't world to draw!" << std::endl;
        return;
    }


    float lableUpdateTime = 0;
    float elapsedTime = 0;
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window_)) {
        glfwSetTime(0);
        glfwPollEvents();

        shownWorld_->update(dt_);
        Render render(shownWorld_);
        render.draw();

        glfwSwapBuffers(window_);
        dt_ = static_cast<float>(glfwGetTime());
        elapsedTime += dt_;
        lableUpdateTime += dt_;
        if (lableUpdateTime > 0.2f) {
            glfwSetWindowTitle(window_, (label_ + " - " + std::to_string(int(1 / dt_)) + " FPS; "
                                         + std::to_string(int(elapsedTime)) + " s; ").c_str());
            lableUpdateTime = 0.0f;
        }
    }
}

void Window::setWorld(const WorldPtr& shownWorld) {
    shownWorld_ = shownWorld;
}






