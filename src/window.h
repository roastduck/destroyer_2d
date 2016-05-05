/**
 * This class interact with GLFW
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GLFW/glfw3.h>
#include "world.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char WINDOW_TITLE[] = "Destroyer2D";

const float BG_COLOR_R = 0.0f;
const float BG_COLOR_G = 0.0f;
const float BG_COLOR_B = 0.0f;

class Window
{
public:
    Window();

    ~Window() noexcept;

    /**
     * Run until the window closes
     */
    void run();

    /**
     * Set the world to display
     * Will keep track to the pointer
     */
    void setWorld(const World *world) { mWorld = world; }

private:
    GLFWwindow *mTarget;
    const World *mWorld;
};

#endif // WORLD_H_

