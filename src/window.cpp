#include <cassert>
#include <iostream>
#include <stdexcept>
#include "image.h"
#include "world.h"
#include "window.h"
#include "render.h"

std::list<Window*> Window::instanceList;

static void error_callback(int error, const char *description)
{
    throw std::runtime_error(description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for (Window *w : Window::instanceList)
        if (w->mTarget == window)
        {
            w->keyDown.insert(key);
            if (action == GLFW_PRESS)
                w->keyPressed.insert(key);
        }
}

Window::Window()
    : mWorld(NULL)
{
    std::cout << "Loading ..." << std::endl;

    instanceList.push_back(this);

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

        glEnable(GL_DEPTH_TEST);
        glDepthRange(-100, 100);
        glDepthFunc(GL_LEQUAL);

        genCursor();
        
        glfwSetKeyCallback(mTarget, key_callback);
    } catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        if (mTarget)
            glfwDestroyWindow(mTarget);
        glfwTerminate();
        throw;
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
    if (mWorld)
        delete mWorld;
    deleteCursor();
    if (mTarget)
        glfwDestroyWindow(mTarget);
    glfwTerminate();
    for (auto i = instanceList.begin(); i != instanceList.end(); i++)
        if (*i == this)
        {
            instanceList.erase(i);
            break;
        }
}

void Window::setWorld(World *world)
{
    if (mWorld)
        delete mWorld;
    mWorld = world;
    world->mWindow = this;
}

void Window::run()
{
    assert(mWorld != NULL);
    while (! glfwWindowShouldClose(mTarget))
    {
        int width, height;
        getWidthHeight(&width, &height);
        glViewport(0, 0, width, height);

        glClearColor(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B, 0.0f);
        glClearDepth(100.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mWorld->setGLOrtho();
        mWorld->step();

        keyPressed.clear();
        keyDown.clear();

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
    }
}

void Window::useCursor(CursorType type)
{
    assert(type < CURSOR_TYPE_NUM);
    glfwSetCursor(mTarget, cursors[type]);
}

void Window::genCursor()
{
    unsigned char pixels[15][15][4];
    GLFWimage image;
    image.width = image.height = 15;
    image.pixels = (unsigned char*)pixels;

    memset(cursors, 0, sizeof cursors);

    memset(pixels, 0, sizeof pixels);
    for (int i=0; i<15; i++)
    {
        memset(pixels[6][i], 0xFF, 4);
        memset(pixels[7][i], 0xFF, 4);
        memset(pixels[8][i], 0xFF, 4);
        memset(pixels[i][6], 0xFF, 4);
        memset(pixels[i][7], 0xFF, 4);
        memset(pixels[i][8], 0xFF, 4);
    }

    cursors[CURSOR_CROSS] = glfwCreateCursor(&image, 7, 7);

    image.width = IMAGES_W[IMAGE_RED_CROSS];
    image.height = IMAGES_H[IMAGE_RED_CROSS];
    image.pixels = (unsigned char *)IMAGES[IMAGE_RED_CROSS];
    cursors[CURSOR_DELETE] = glfwCreateCursor(&image, 7, 7);
}

void Window::deleteCursor() noexcept
{
    for (int i = 0; i < CURSOR_TYPE_NUM; i++)
        if (cursors[i])
            glfwDestroyCursor(cursors[i]);
}

