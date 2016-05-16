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

/**
 * Handle according to mouse behaviors
 * leftClick and rightClick will be triggered only once in
 * a time interval.
 * When mouse button actually released, leftRelease or
 * rightRelease will be triggered.
 * move will be triggered when all above are not triggerd.
 */
class MouseCallback
{
public:
    MouseCallback(MouseHandler *_handler) : mMouseHandler(_handler) {}
    virtual ~MouseCallback() {}

    virtual void leftClick(float x, float y) {}
    virtual void rightClick(float x, float y) {}
    virtual void leftRelease(float x, float y) {}
    virtual void rightRelease(float x, float y) {}
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
        {
            toPut->setIsUserCreated(true);
            if (toPut->shouldBind())
                mMouseHandler->getWorld()->requireKey(toPut);
            createNew(x, y);
        } else
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
            if (b2Vec2(sx - x, sy - y).Length() < 1.5f * STICK_END_THICKNESS) return;
            drawing = false;
            ToPut *p = new ToPut(mMouseHandler->getWorld(), sx, sy, x, y);
            if (! p->tryPutDown())
                delete p;
            else
                p->setIsUserCreated(true);
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

class DeletingCallback : public MouseCallback
{
public:
    DeletingCallback(MouseHandler *_handler) : MouseCallback(_handler)
    {
        mMouseHandler->getWorld()->getWindow()->useCursor(Window::CURSOR_DELETE);
    }

    ~DeletingCallback()
    {
        mMouseHandler->getWorld()->getWindow()->useCursor(Window::CURSOR_ARROW);
    }
   
    void leftClick(float x, float y) override
    {
        World::GlobalTestPoint callback(b2Vec2(x, y));
        b2AABB aabb;
        aabb.lowerBound = b2Vec2(x - 0.001f, y - 0.001f);
        aabb.upperBound = b2Vec2(x + 0.001f, y + 0.001f);
        mMouseHandler->getWorld()->getB2World()->QueryAABB(&callback, aabb);
        if (callback.fixture && ((Matter*)(callback.fixture->GetBody()->GetUserData()))->getIsUserCreated())
            delete (Matter*)(callback.fixture->GetBody()->GetUserData());
    }

    void rightClick(float x, float y) override
    {
        mMouseHandler->setStatus(MouseHandler::MOUSE_FREE);
    }
};

class DeleteButtonCallback : public MouseCallback
{
public:
    DeleteButtonCallback(MouseHandler *_handler) : MouseCallback(_handler) {}

    void leftClick(float, float) override
    {
        mMouseHandler->setStatus(MouseHandler::MOUSE_PUTTING, new DeletingCallback(mMouseHandler));
    }
};

class DraggingCallback : public MouseCallback
{
    typedef b2MouseJoint *JointPointer;
public:
    class LocalListener : public b2DestructionListener
    {
    public:
        LocalListener(JointPointer &_joint) : joint(_joint) {}

        void SayGoodbye(b2Joint* _joint) override
        {
            if (joint == _joint) joint = 0;
        }
        void SayGoodbye(b2Fixture*) override {}

    private:
        JointPointer &joint;
    } localListener;

    DraggingCallback(MouseHandler *_handler) : MouseCallback(_handler), localListener(joint), joint(NULL)
    {
        World::myDestructionListener.subscribe(&localListener);
    }

    ~DraggingCallback()
    {
        if (joint)
            mMouseHandler->getWorld()->getB2World()->DestroyJoint(joint);
        World::myDestructionListener.unsubscribe(&localListener);
    }

    void leftClick(float x, float y) override
    {
        if (joint != NULL) return;

        World::GlobalTestPoint callback(b2Vec2(x, y));
        b2AABB aabb;
        aabb.lowerBound = b2Vec2(x - 0.001f, y - 0.001f);
        aabb.upperBound = b2Vec2(x + 0.001f, y + 0.001f);
        mMouseHandler->getWorld()->getB2World()->QueryAABB(&callback, aabb);
        if (! callback.fixture || callback.fixture->GetBody()->GetType() != b2_dynamicBody) return;

        b2MouseJointDef jointDef;
        jointDef.target.Set(x, y);
        jointDef.bodyA = mMouseHandler->getWorld()->getFrameBody();
        jointDef.bodyB = callback.fixture->GetBody();
        jointDef.maxForce = 1000.0f * callback.fixture->GetBody()->GetMass();
        joint = (JointPointer)(mMouseHandler->getWorld()->getB2World()->CreateJoint(&jointDef));
    }

    void leftRelease(float, float) override
    {
        if (joint)
        {
            mMouseHandler->getWorld()->getB2World()->DestroyJoint(joint);
            joint = NULL;
        }
    }

    void move(float x, float y) override
    {
        if (joint)
            joint->SetTarget(b2Vec2(x, y));
    }

private:
    JointPointer joint;
};

#endif // MOUSECALLBACK_H_
