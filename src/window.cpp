#include <cassert>
#include <stdexcept>
#include "window.h"

static void error_callback(int error, const char *description)
{
    throw std::runtime_error(description);
}

Window::Window()
    : mWorld(NULL)
{
    if (! glfwInit())
        throw std::runtime_error("Error loading GLFW");

    try
    {
        glfwSetErrorCallback(error_callback);

        mTarget = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
        if (! mTarget)
            throw std::runtime_error("Error Creating window");

        glfwMakeContextCurrent(mTarget);
    } catch (...)
    {
        if (mTarget)
            glfwDestroyWindow(mTarget);
        glfwTerminate();
        throw;
    }
}

Window::~Window() noexcept
{
    if (mTarget)
        glfwDestroyWindow(mTarget);
    glfwTerminate();
}

void Window::run()
{
    assert(mWorld != NULL);
    while (! glfwWindowShouldClose(mTarget))
    {
        int width, height;
        glfwGetFramebufferSize(mTarget, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B, 0);
        glClear(GL_COLOR_BUFFER_BIT); // we don't need depth test

        mWorld->setGLOrtho();
        mWorld->drawAll();

        glfwSwapBuffers(mTarget);
        glfwPollEvents();
    }
}

