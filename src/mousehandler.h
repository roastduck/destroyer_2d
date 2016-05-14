/**
 * Forwards mouse actions to Matter
 */

#ifndef MOUSEHANDLER_H_
#define MOUSEHANDLER_H_

#include <ctime>
#include <cstdlib>
#include <list>
#include <utility>
#include <Box2D/Box2D.h>

class World;
class Rigid;

class MouseCallback;

class MouseHandler
{
public:
    enum MouseStatus
    {
        MOUSE_FREE = 0, // free to click button
        MOUSE_PUTTING = 1, // putting objects into world
    };

    MouseHandler(World *_world);

    /**
     * add a button to the button list
     */
    void addButton(Rigid *_rigid, MouseCallback *_callback);

    /**
     * main entry to process
     */
    void process();

    /**
     * update current mouse status from GLFW
     * should be public for World to set it as friend
     */
    void updateMouse();

    /**
     * Set callback that used at MOUSE_FREE when is not on buttons.
     * This callback will NOT be deleted when switching status.
     */
    void setFreeCallback(MouseCallback *callback);

    /**
     * Set callback that used at MOUSE_PUTTING.
     * This callback will be deleted when switching status.
     */
    void setPuttingCallback(MouseCallback *callback);

    /**
     * set woring status
     * callback used for corresponding status can be set through the 2nd parameter,
     * ONLY if it is not NULL.
     */
    void setStatus(MouseStatus _status, MouseCallback *callback = NULL);

    World *getWorld() const { return mWorld; }

private:
    void triggerCallback(MouseCallback *callback);

    void processFree();
    void processPutting();

    void reset();

    World *mWorld;

    float worldX, worldY; // cursor in world coordinates
    bool leftClicked, rightClicked, leftHold, rightHold, leftRelease, rightRelease;
    clock_t lastClickedClock;

    MouseStatus status;

    std::list< std::pair<Rigid*, MouseCallback*> > buttons;
    MouseCallback *mFreeCallback, *mPuttingCallback;
};

#endif // MOUSEHANDLER_H_

