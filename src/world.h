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
     * Set part of the whole world as current view.
     */
    void setView(float l, float r, float d, float u);

    /**
     * Set OpenGL with orthographic projection
     * To disply corresponding area
     */
    void setGLOrtho() const;

    /**
     * Corresponding LiquidFun Object
     */
    b2World *getB2World() const { return physics; }

    /**
     * Run next simulation step
     */
    void step();

    /**
     * Get 1 pixel = ? meter
     */
    float getScale() const;

    /**
     * Framebody can be used as a anchor
     */
    b2Body *getFrameBody() const { return frameBody; }

    /**
     * Return b2World pointer
     */
    b2World *getReferee() const { return physics; }

    Window *getWindow() const { return mWindow; }
    
    /**
     * Manger for all destruction listener
     */
    class MyDestructionListener final : public b2DestructionListener
    {
    public:
        void SayGoodbye(b2Joint* joint) override;
        void SayGoodbye(b2Fixture *fixture) override;
        void SayGoodbye(b2ParticleGroup *group) override;
        // we will not manipulate single particle
        void subscribe(b2DestructionListener *p);
        void unsubscribe(b2DestructionListener *p);
    private:
        std::list<b2DestructionListener*> subscribers;
    };
    static MyDestructionListener myDestructionListener;

    /**
     * A helper for finding a fixture overlapping with a point
     */
    class GlobalTestPoint final : public b2QueryCallback
    {
    public:
        GlobalTestPoint(const b2Vec2 &_p) : p(_p), fixture(NULL) {}
        bool ReportFixture(b2Fixture *_f) override;
        b2Vec2 p;
        b2Fixture *fixture; /// the answer
    };

protected:
    friend void Window::setWorld(World*);
    friend void MouseHandler::updateMouse();

    /**
     * Draw every Matter into OpenGL
     * Things can be drawn out of drawAll
     */
    virtual void drawAll() const noexcept;

    Window *mWindow;
    MouseHandler *mMouseHandler;

    // borders of the whole world and the displayed part in world coordinates
    float mLeftMost, mRightMost, mDownMost, mUpMost;
    float mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost;
    
    // LiquidFun b2world object
    b2World *physics;

    // outer frame
    b2Body *frameBody;
};

/**
 * This world is the main scenery of the game.
 * Players first build a ship in a limited area.
 * When clicked Battle, he will be set free and play.
 */
class MainWorld : public World
{
public:
    MainWorld();
    ~MainWorld();

    enum BUTTON_NAME
    {
        BUTTON_SMALL_WOOD_BLOCK = 0,
        BUTTON_LARGE_WOOD_BLOCK = 1,
        BUTTON_SMALL_STEEL_BALL = 2,
        BUTTON_STEEL_STICK = 3,
        BUTTON_DELETE = 4,

        BUTTON_NUM = 5
    };

private:
    static constexpr float BUILD_W = 30.0f, BUILD_H = 23.0f;
    static constexpr float BATTLE_W = 100.0f, BATTLE_H = 40.0f;

    std::pair<Rigid*, MouseCallback*> buttons[BUTTON_NUM];
    MouseCallback *draggingCallback;
    Rigid *buildFrame;
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

