//
// Created by boris on 11/13/2021.
//

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "ve_window.h"
#include "stdlibraries.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "render/ve_render.h"

using namespace VE;

Window::Window(int width, int height, const std::string &label) : width_(width), height_(height), label_(label) {
    windowInitialization(width, height, label);
    guiInitialization();
}

void Window::windowInitialization(int width, int height, const std::string &label) {
    glfwInit();
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window_ = glfwCreateWindow(width, height, label.c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }
    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, this);
    setCallbackFunction();
    windowAspectRatio_ = static_cast<float>(width) / static_cast<float>(height);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }
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
    using namespace std;
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

    Render render(windowAspectRatio_);

    //glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClearColor(0.5f, 0.7f, 0.95f, 0.0f);
    glClearDepth(1.0f);

    chrono::duration<double> frameTime(0.0);
    chrono::duration<double> sleepAdjust(0.0);
    chrono::duration<double> lableUpdateTime(0.0);

    while (!glfwWindowShouldClose(window_)) {
        auto t1 = chrono::steady_clock::now();

        glfwGetWindowSize(window_, &width_, &height_);

        static int bufferWidth = 0;
        static int bufferHeight = 0;
        glfwGetFramebufferSize(window_, &bufferWidth, &bufferHeight);
        glViewport(0, 0, bufferWidth, bufferHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto startWorldUpdateTime = chrono::steady_clock::now();
        world_->update(deltaTime_);
        chrono::duration<double> worldUpdateTime = chrono::steady_clock::now() - startWorldUpdateTime;

        auto startRenderTime = chrono::steady_clock::now();
        render.draw(world_);

        if (mouse_->isLock()) {
            Vector3 lockScreenPosition = openGLToScreenCoordinate(mouse_->lockPosition());
            glfwSetCursorPos(window_, lockScreenPosition.x, lockScreenPosition.y);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        chrono::duration<double> renderTime = chrono::steady_clock::now() - startRenderTime;

        chrono::duration<double> target(deltaTime_);
        chrono::duration<double> timeUsed = chrono::steady_clock::now() - t1;
        if (chrono::steady_clock::now() - t1 < target) {
            while (chrono::steady_clock::now() - t1 < target) {
                this_thread::yield();
            }
        } else {
            frameTime = chrono::steady_clock::now() - t1;
            std::string info = "Freez info: " +
                               to_string(int(1 / frameTime.count())) + " (" + to_string(frameTime.count() * 1000.0f) + ")" + " FPS | " +
                               to_string(timeUsed.count() * 1000.0f) + " ms time used | " +
                               to_string(worldUpdateTime.count() * 1000.0f) + " ms world update | " +
                               to_string(renderTime.count() * 1000.0f) + " ms render | ";
            std::cout << info << std::endl;
        }

        frameTime = chrono::steady_clock::now() - t1;
        lableUpdateTime += frameTime;
        if (lableUpdateTime > chrono::duration<double>(0.2)) {
            lableUpdateTime = chrono::duration<double>(0);
            glfwSetWindowTitle(window_, (label_ + " | " +
                                         to_string(int(1 / frameTime.count())) + " (" + to_string(frameTime.count() * 1000.0f) + ")" + " FPS | " +
                                         to_string(timeUsed.count() * 1000.0f) + " ms time used | " +
                                         to_string(worldUpdateTime.count() * 1000.0f) + " ms world update | " +
                                         to_string(renderTime.count() * 1000.0f) + " ms render | ").c_str());

        }
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

void Window::cursorChangePositionEvent(const VE::Vector3 &cursorPosition) {
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
        VE::Vector3 openGLCoordinate = getThis(glfwwindow)->screenToOpenGLCoordinate(VE::Vector3(xpos, ypos));
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

Vector3 Window::screenToOpenGLCoordinate(const Vector3 &screenCoordinate) {
    float xScale = width_ / (windowAspectRatio_ * 2);
    float yScale = height_ / 2;
    return Vector3(screenCoordinate.x / xScale - windowAspectRatio_, 1 - screenCoordinate.y / yScale);
}

Vector3 Window::openGLToScreenCoordinate(const Vector3 &openGLCoordinate) {
    float xScale = width_ / (windowAspectRatio_ * 2);
    float yScale = height_ / 2;
    return Vector3((windowAspectRatio_ + openGLCoordinate.x) * xScale,
                   (1 - openGLCoordinate.y) * yScale);
}






