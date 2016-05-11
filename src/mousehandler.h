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
#include "const.h"

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

class MouseCallback
{
public:
    MouseCallback(MouseHandler *_handler) : mMouseHandler(_handler) {}
    virtual ~MouseCallback() {}

    virtual void leftClick(float x, float y) {}
    virtual void rightClick(float x, float y) {}
    virtual void move(float x, float y) {}
protected:
    MouseHandler *mMouseHandler;
};

template <class ToPut>
class PuttingCallback : public MouseCallback
{
public:
    PuttingCallback(MouseHandler *_handler, float x, float y)
        : MouseCallback(_handler)
    {
        toPut = new ToPut(mMouseHandler->getWorld(), x, y);
        toPut->getReferee()->SetType(b2_staticBody);
        toPut->getReferee()->SetActive(false);
        toPut->setDepth(-1.0f);
        toPut->setDefalutAlert(ALERT_SHADOW);
    }

    void leftClick(float x, float y) override
    {
        if (toPut->tryPutDown())
            mMouseHandler->setStatus(MouseHandler::MOUSE_FREE);
        else
            toPut->setAlert(ALERT_WARNING, CLOCKS_PER_SEC);
    }

    void rightClick(float x, float y) override
    {
        delete toPut;
        mMouseHandler->setStatus(MouseHandler::MOUSE_FREE);
    }

    void move(float x, float y) override
    {
        toPut->tryMoveTo(x, y, toPut->getReferee()->GetAngle());
    }

private:
    ToPut *toPut;
};

template <class ToPut>
class NewObjectCallback : public MouseCallback
{
public:
    NewObjectCallback(MouseHandler *_handler) : MouseCallback(_handler) {}

    void leftClick(float x, float y) override
    {
        mMouseHandler->setStatus(MouseHandler::MOUSE_PUTTING, new PuttingCallback<ToPut>(mMouseHandler, x, y));
    }
};

#endif // MOUSEHANDLER_H_

