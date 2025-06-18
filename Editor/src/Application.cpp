
#include "Application.h"

#include <assert.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application::~Application() {
    Shutdown();
}

bool Application::Init() {
    glfwSetErrorCallback(GlfwErrorCallback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    mWindow = glfwCreateWindow(1280, 720, "GBA Texture Editor", nullptr, nullptr);
    if (!mWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glfwSwapInterval(1);

    return true;
}

void Application::Shutdown() {
    glfwTerminate();
    mWindow = nullptr;
    mImGuiInitialized = false;
}

void Application::Run() {
    if (!Init()) {
        Shutdown();
        return;
    }

    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();

        glfwSwapBuffers(mWindow);
    }

    Shutdown();
}

void Application::GlfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}
