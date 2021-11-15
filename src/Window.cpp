//
// Created by boris on 11/13/2021.
//

#include "Window.h"
#include "Render.h"
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
    Render render(shownWorld_);

    float lableUpdateTime = 0;
    float elapsedTime = 0;
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window_)) {
        glfwSetTime(0);
        glfwPollEvents();

        shownWorld_->update(dt_);
        render();

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

void Window::setWorld(WorldPtr shownWorld) {
    shownWorld_ = shownWorld;
}






/*
struct PointFLoat {
    float x = 0;
    float y = 0;
    float z = 0;
};






void printAxis(float axisLen = 10){
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(axisLen, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, axisLen, 0);

    glColor3f(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, axisLen);
    glEnd();
}

void PrintCube() {
    float vertices[] = {0, 0, 0,
                        1, 0, 0,
                        1, 1, 0,
                        0, 1, 0,
                        0, 0, 1,
                        1, 0, 1,
                        1, 1, 1,
                        0, 1, 1};
    unsigned int indices[6][4] = {{3, 2, 1, 0},
                              {4, 5, 6, 7},
                              {0, 1, 5, 4},
                              {2, 3, 7, 6},
                              {3, 0, 4, 7},
                              {1, 2, 6, 5}};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glPushMatrix();

    glTranslatef(1, 1, 0);
    glRotatef(45,1,0,0);
    glScalef(2, 2, 2);

    for(int i = 0; i < 6; i++) {
        glColor3f(0.50f + i/100.0, 0.50f + i/100.0, 0.50f + i/100.0);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, indices[i]);
    }
    printAxis(2);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
};


void ShowWorld() {
    float floorVertices[] = {1, 1, 0,
                             -1, 1, 0,
                             -1, -1, 0,
                             1, -1, 0};

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, floorVertices);

    for (int i = -5; i < 5; i++) {
        for (int j = -5; j < 5; j++) {
            glPushMatrix();
            if ((i + j) % 2) glColor3f(0, 0.5, 0);
            else glColor3f(1, 1, 1);
            glTranslated(i * 2, j * 2, 0);
            glDrawArrays(GL_POLYGON, 0, 4);
            glPopMatrix();
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    printAxis();
}


float xAlfa = 70;
float zAlfa = 40;

PointFLoat playerPosition = {6.5, -7.5, 1};

void MoveCamera() {
    const float gravity = 0.05f;

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);

    glTranslated(-playerPosition.x, -playerPosition.y, -3 - playerPosition.z);
}

void Window::run() {
    glEnable(GL_DEPTH_TEST);


    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClearDepth(1.0f);

    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 2, 80);


    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        MoveCamera();
        ShowWorld();
        PrintCube();
        glPopMatrix();

        glfwSwapBuffers(window_);
    }
}

void Window::keyboardEvent(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UP) xAlfa = (xAlfa += 3) > 180 ? 180 : xAlfa;
    if (key == GLFW_KEY_DOWN) xAlfa = (xAlfa -= 3) < 0 ? 0 : xAlfa;
    if (key == GLFW_KEY_LEFT) zAlfa = (zAlfa += 3);
    if (key == GLFW_KEY_RIGHT) zAlfa = (zAlfa -= 3);

    float spead = 0;
    float speadAngleInRadian = -zAlfa / 180 * M_PI;

    if (key == GLFW_KEY_W) spead = 0.3;
    if (key == GLFW_KEY_S) spead = -0.3;
    if (key == GLFW_KEY_A) {
        spead = 0.3;
        speadAngleInRadian -= M_PI * 0.5f;
    }
    if (key == GLFW_KEY_D) {
        spead = 0.3;
        speadAngleInRadian += M_PI * 0.5f;
    }
    playerPosition.x += spead * sin(speadAngleInRadian);
    playerPosition.y += spead * cos(speadAngleInRadian);
}

void Window::setCallbackFunction() {
    auto keyboardEventFun = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        getThis(window)->keyboardEvent(key, scancode, action, mods);
    };
    glfwSetKeyCallback(window_, keyboardEventFun);
}

Window::~Window() {

}

*/






