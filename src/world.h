/**
 * All object in a scenario
 * Interact with OpenGL and LiquidFun
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <Box2D/Box2D.h>
#include "window.h"
#include "mousehandler.h"

class World
{
public:
    /**
     * Initialize with the border of whole world
     */
    World(float leftMost, float rightMost, float downMost, float upMost) noexcept;

    virtual ~World() noexcept;

    /**
     * Set OpenGL with orthographic projection
     * To disply corresponding area
     */
    void setGLOrtho() const;

    /**
     * Draw everything into OpenGL
     */
    virtual void drawAll() const noexcept;

    /**
     * Corresponding LiquidFun Object
     */
    b2World *getB2World() const { return physics; }

    /**
     * Run next simulation step
     */
    void step();

    /**
     * Return b2World pointer
     */
    b2World *getReferee() const { return physics; }
    
protected:
    friend void Window::setWorld(World*);
    friend void MouseHandler::updateMouse();

    Window *mWindow;
    MouseHandler *mMouseHandler;

    // borders of the whole world and the displayed part in world coordinates
    float mLeftMost, mRightMost, mDownMost, mUpMost;
    float mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost;
    
    // LiquidFun b2world object
    b2World *physics;
};

// Methods for particle systems are implemented in matter.cpp
class MyDestructionListener : public b2DestructionListener
{
public:
    void SayGoodbye(b2Joint* joint) override {}
    void SayGoodbye(b2Fixture *fixture) override {}
    void SayGoodbye(b2ParticleGroup *group) override;
};

#ifdef COMPILE_TEST

/**
 * display a triangle
 */
class TestWorldDisplayTriangle : public World
{
public:
    TestWorldDisplayTriangle();
    void drawAll() const noexcept;
};

/**
 * display a block falling down into water
 */
class TestWorldSimplePhysics : public World
{
public:
    TestWorldSimplePhysics();
};

/**
 * display buttons
 */
class TestWorldButtons : public World
{
public:
    TestWorldButtons();
    ~TestWorldButtons();
private:
    MouseCallback *callback;
};

#endif // COMPILE_TEST

#endif // WORLD_H_

