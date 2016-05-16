/**
 * This class interact with GLFW
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <list>
#include <unordered_set>
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
        CURSOR_DELETE = 2,

        CURSOR_TYPE_NUM = 3
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

    /**
     * If any key being pressed (not pressed last time)? Return 0 or the key.
     */
    int isKeyPressed() const { return keyPressed.empty() ? 0 : *(keyPressed.begin()); }

    /**
     * If the given key being pressed?
     */
    bool isKeyPressed(int key) const { return keyPressed.count(key); }

    /**
     * If any key down? Return 0 or the key.
     */
    int isKeyDown() const { return keyDown.empty() ? 0 : *(keyDown.begin()); }

    /**
     * If the given key down?
     */
    bool isKeyDown(int key) const { return keyDown.count(key); }

private:
    friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void genCursor();
    void deleteCursor() noexcept;

    std::unordered_set<int> keyPressed, keyDown;

    GLFWcursor *cursors[CURSOR_TYPE_NUM];

    GLFWwindow *mTarget;
    World *mWorld;

    static std::list<Window*> instanceList;
};

#endif // WORLD_H_

