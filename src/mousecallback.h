/**
 * This file consists of template classes of
 * mouse callback listeners
 */

#ifndef MOUSECALLBACK_H_
#define MOUSECALLBACK_H_

#include "world.h"
#include "const.h"
#include "window.h"
#include "render.h"
#include "mousehandler.h"

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

/**
 * Callbacks of putting normal things that ! canDrawLine()
 */
template <class ToPut>
class PuttingCallback : public MouseCallback
{
public:
    PuttingCallback(MouseHandler *_handler, float x, float y)
        : MouseCallback(_handler)
    {
        createNew(x, y);
    }

    void leftClick(float x, float y) override
    {
        if (toPut->tryPutDown())
            createNew(x, y);
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

    void createNew(float x, float y)
    {
        toPut = new ToPut(mMouseHandler->getWorld(), x, y);
        toPut->getReferee()->SetType(b2_staticBody);
        toPut->getReferee()->SetActive(false);
        toPut->setDepth(-1.0f);
        toPut->setDefalutAlert(ALERT_SHADOW);
    }
};

/**
 * Putting rigids with canDrawLine()
 */
template <class ToPut>
class PuttingLineCallback : public MouseCallback
{
public:
    PuttingLineCallback(MouseHandler *_handler, float x, float y)
        : MouseCallback(_handler), drawing(false)
    {
        mMouseHandler->getWorld()->getWindow()->useCursor(Window::CURSOR_CROSS);
    }

    ~PuttingLineCallback()
    {
        mMouseHandler->getWorld()->getWindow()->useCursor(Window::CURSOR_ARROW);
    }

    void leftClick(float x, float y) override
    {
        if (drawing)
        {
            drawing = false;
            new ToPut(mMouseHandler->getWorld(), sx, sy, x, y);
        } else
        {
            drawing = true;
            sx = x, sy = y;
        }
    }

    void rightClick(float x, float y) override
    {
        mMouseHandler->setStatus(MouseHandler::MOUSE_FREE);
    }

    void move(float x, float y) override
    {
        if (drawing)
            Render::getInstance().drawLine(sx, sy, x, y);
    }

private:
    float sx, sy; // starting point
    bool drawing;
};

template <class ToPut>
class NewObjectCallback : public MouseCallback
{
public:
    NewObjectCallback(MouseHandler *_handler) : MouseCallback(_handler) {}

    void leftClick(float x, float y) override
    {
        MouseCallback *callback;
        if (ToPut::canDrawLine())
            callback = new PuttingLineCallback<ToPut>(mMouseHandler, x, y);
        else
            callback = new PuttingCallback<ToPut>(mMouseHandler, x, y);
        // callback will be deleted when switching mouse status
        mMouseHandler->setStatus(MouseHandler::MOUSE_PUTTING, callback);
    }
};

#endif // MOUSECALLBACK_H_
