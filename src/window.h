/**
 * This class interact with GLFW
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GLFW/glfw3.h>
#include "world.h"
#include "const.h"

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
    void setWorld(World *world) { mWorld = world; }

private:
    GLFWwindow *mTarget;
    World *mWorld;
};

#endif // WORLD_H_

