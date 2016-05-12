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
     * set woring status
     * callback used for MOUSE_PUTTING can be set through the 2nd parameter, and
     * will be deleted in `reset`.
     */
    void setStatus(MouseStatus _status, MouseCallback *puttingCallback = NULL);

    World *getWorld() const { return mWorld; }

private:
    void processFree();
    void processPutting();

    void reset();

    World *mWorld;

    float worldX, worldY; // cursor in world coordinates
    bool leftClicked, rightClicked;
    clock_t lastClickedClock;

    MouseStatus status;

    std::list< std::pair<Rigid*, MouseCallback*> > buttons;
    MouseCallback *mPuttingCallback;
};

#endif // MOUSEHANDLER_H_

