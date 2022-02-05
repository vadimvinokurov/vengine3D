//
// Created by boris on 11/13/2021.
//

#include "ve_window.h"
#include "ve_render.h"

#include <stdexcept>
#include <cmath>

using namespace VE;

Window::Window(int width, int height, const std::string &label) : width_(width), height_(height), label_(label) {
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
    setCallbackFunction();
    windowAspectRatio_ = static_cast<float>(width) / static_cast<float>(height);
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
    if (world_ == nullptr) {
        std::cerr << "Window hasn't world to draw!" << std::endl;
        return;
    }
    if (keyboard_ == nullptr) {
        std::cerr << "Window hasn't keyboard to control!" << std::endl;
        return;
    }
    if (mouse_ == nullptr) {
        std::cerr << "Window hasn't mouse to control!" << std::endl;
        return;
    }

    glfwSwapInterval(1);
    float lableUpdateTime = 0;
    float elapsedTime = 0;
    Render render(windowAspectRatio_);
    while (!glfwWindowShouldClose(window_)) {
        glfwSetTime(0);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render.draw(world_);
        world_->update(dt_);


        if (mouse_->isLock()) {
            Vector lockScreenPosition = openGLToScreenCoordinate(mouse_->lockPosition());
            glfwSetCursorPos(window_, lockScreenPosition.x, lockScreenPosition.y);
            //glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        } else {
            //glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        dt_ = static_cast<float>(glfwGetTime());
        elapsedTime += dt_;
        lableUpdateTime += dt_;
        if (lableUpdateTime > 0.2f) {
            glfwSetWindowTitle(window_, (label_ + " - " + std::to_string(int(1 / dt_)) + " FPS; "
                                         + std::to_string(int(elapsedTime)) + " s; ").c_str());
            lableUpdateTime = 0.0f;
        }
        dt_ = 1.0f / 75.0f;
    }
}

void Window::setWorld(const WorldPtr &shownWorld) {
    world_ = shownWorld;
}

void Window::setHid(const VE::KeyboardPtr &keyboard, const VE::MousePtr &mouse) {
    keyboard_ = keyboard;
    mouse_ = mouse;
}

void Window::mouseButtonEvent(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        mouse_->setKeyState(button, VE_KEY_PRESSED);
    } else if (action == GLFW_REPEAT) {
        mouse_->setKeyState(button, VE_KEY_REPEAT);
    } else if (action == GLFW_RELEASE) {
        mouse_->setKeyState(button, VE_KEY_RELEASE);
    }
}

void Window::cursorChangePositionEvent(const VE::Vector &cursorPosition) {
    mouse_->setPosition(cursorPosition);
}

void Window::keyEvent(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keyboard_->setKeyState(key, VE_KEY_PRESSED);
    } else if (action == GLFW_REPEAT) {
        keyboard_->setKeyState(key, VE_KEY_REPEAT);
    } else if (action == GLFW_RELEASE) {
        keyboard_->setKeyState(key, VE_KEY_RELEASE);
    }
}

void Window::setCallbackFunction() {
    auto cursor_callback = [](GLFWwindow *glfwwindow, double xpos, double ypos) {
        VE::Vector openGLCoordinate = getThis(glfwwindow)->screenToOpenGLCoordinate(VE::Vector(xpos, ypos));
        getThis(glfwwindow)->cursorChangePositionEvent(openGLCoordinate);
    };
    glfwSetCursorPosCallback(window_, cursor_callback);

    auto mouseButton_callback = [](GLFWwindow *glfwwindow, int button, int action, int mods) {
        getThis(glfwwindow)->mouseButtonEvent(button, action, mods);
    };
    glfwSetMouseButtonCallback(window_, mouseButton_callback);

    auto keyEventFun = [](GLFWwindow *glfwwindow, int key, int scancode, int action, int mods) {
        getThis(glfwwindow)->keyEvent(key, scancode, action, mods);
    };
    glfwSetKeyCallback(window_, keyEventFun);
}

Vector Window::screenToOpenGLCoordinate(const Vector &screenCoordinate) {
    float xScale = width_ / (windowAspectRatio_ * 2);
    float yScale = height_ / 2;
    return Vector(screenCoordinate.x / xScale - windowAspectRatio_, 1 - screenCoordinate.y / yScale);
}

Vector Window::openGLToScreenCoordinate(const Vector &openGLCoordinate) {
    float xScale = width_ / (windowAspectRatio_ * 2);
    float yScale = height_ / 2;
    return Vector((windowAspectRatio_ + openGLCoordinate.x) * xScale,
                  (1 - openGLCoordinate.y) * yScale);
}






