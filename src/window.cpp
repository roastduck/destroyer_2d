#include <cassert>
#include <iostream>
#include <stdexcept>
#include "window.h"
#include "render.h"

static void error_callback(int error, const char *description)
{
    throw std::runtime_error(description);
}

Window::Window()
    : mWorld(NULL)
{
    std::cout << "Loading ..." << std::endl;

    if (! glfwInit())
        throw std::runtime_error("Error loading GLFW");

    try
    {
        glfwSetErrorCallback(error_callback);

        mTarget = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
        if (! mTarget)
            throw std::runtime_error("Error Creating window");

        glfwMakeContextCurrent(mTarget);

        load_extensions();

        Render::setWindow(this);
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
        getWidthHeight(&width, &height);
        glViewport(0, 0, width, height);

        glClearColor(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // we don't need depth test

        mWorld->setGLOrtho();
        mWorld->drawAll();

        switch (glGetError())
        {
        case GL_INVALID_ENUM:
            throw std::runtime_error("GL_INVALID_ENUM");
        case GL_INVALID_VALUE:
            throw std::runtime_error("GL_INVALID_VALUE");
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            throw std::runtime_error("GL_INVALID_FRAMEBUFFER_OPERATION");
        case GL_OUT_OF_MEMORY:
            throw std::runtime_error("GL_OUT_OF_MEMORY");
        case GL_STACK_UNDERFLOW:
            throw std::runtime_error("GL_STACK_UNDERFLOW");
        case GL_STACK_OVERFLOW:
            throw std::runtime_error("GL_STACK_OVERFLOW");
        }

        glfwSwapBuffers(mTarget);
        glfwPollEvents();

        mWorld->step();
    }
}

