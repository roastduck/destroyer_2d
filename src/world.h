/**
 * All object in a scenario
 * Interact with OpenGL and LiquidFun
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <Box2D/Box2D.h>

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
    
protected:
    // borders of the whole world and the displayed part in pixels
    float mLeftMost, mRightMost, mDownMost, mUpMost;
    float mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost;
    
    // LiquidFun b2world object
    b2World *physics;
};

// Methods for particle systems are implemented in matter.cpp
class MyDestructionListener : public b2DestructionListener
{
public:
    void SayGoodbye(b2Joint* joint) {}
    void SayGoodbye(b2Fixture *fixture) {}
    void SayGoodbye(b2ParticleGroup *group);
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

#endif // COMPILE_TEST

#endif // WORLD_H_

