/**
 * This class interact with GLFW
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "loader.h"
#include "const.h"

class World;

class Window
{
public:
    enum CursorType
    {
        CURSOR_ARROW = 0,
        CURSOR_CROSS = 1,

        CURSOR_TYPE_NUM = 2
    };

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
    void setWorld(World *world);

    /**
     * Get current window width and height
     */
    void getWidthHeight(int *width, int *height) const { glfwGetFramebufferSize(mTarget, width, height); }

    /**
     * Return GLFWwindow pointer
     */
    GLFWwindow *getReferee() const { return mTarget; }

    /**
     * Choose a cursor type
     */
    void useCursor(CursorType type);

private:
    void genCursor();
    void deleteCursor() noexcept;

    GLFWcursor *cursors[CURSOR_TYPE_NUM];

    GLFWwindow *mTarget;
    World *mWorld;
};

#endif // WORLD_H_

