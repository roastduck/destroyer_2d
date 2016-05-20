#ifndef WORLD_H_
#define WORLD_H_

#include <unordered_map>
#include <vector>
#include <string>
#include <Box2D/Box2D.h>
#include "window.h"
#include "mousehandler.h"

class Enemy;

/**
 * All object in a scenario
 * Interact with OpenGL and LiquidFun
 */
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
    virtual void setView(float l, float r, float d, float u); /// will be overriden in test classes

    /**
     * Set OpenGL with orthographic projection
     * To disply corresponding area
     */
    void setGLOrtho() const;

    /**
     * Run next simulation step
     */
    virtual void step();

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
     * Require to bind a key to Rigid r
     */
    void requireKey(Rigid *r);
    
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
    virtual void drawAll() const noexcept; /// will be overriden in test classes

    /**
     * Display a popup window until calling cancelPopup()
     * located with lower-left (l,d) and upper-right(r,u)
     * in world coordinates
     * s = "" means not to display
     */
    void displayPopup(const std::string &s, float l, float r, float d, float u);

    /**
     * Cancel popup window
     */
    void cancelPopup();

    /**
     * What is displaying in popup window?
     * s = "" means not displaying
     */
    const std::string &getPopup() { return popupMsg; }

    /**
     * Is player p's key down?
     */
    virtual bool playerKeyDown(int p, int key) { assert(p == -1); return mWindow->isKeyDown(key); }

    Window *mWindow;
    MouseHandler *mMouseHandler;

    // borders of the whole world and the displayed part in world coordinates
    float mLeftMost, mRightMost, mDownMost, mUpMost;
    float mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost;
    float mx, my; // center of x and y axises
    
    // LiquidFun b2world object
    b2World *physics;

private:
    /**
     * Examine contacts and make danmages
     */
    void examContact();

    /**
     * Check keyboard Actions
     */
    void checkKeyboard();

    class MyContactListener final : public b2ContactListener
    {
    public:
        void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;
    private:
        friend void World::examContact();
        std::vector<Rigid*> destroying;
    } myContactListener;

    // popup window
    std::string popupMsg;
    float popupL, popupR, popupD, popupU;

    // outer frame
    b2Body *frameBody;

    Rigid *requiringKey;
    std::string bindMsg =
        "  PLEASE CHOOSE A KEY TO HANDLE THIS OBJECT.\n"
        "\n"
        "  ...MUST BE A LETTER...\n";
};

/**
 * This world is the main scenery of the game.
 * Players first build a ship in a limited area.
 * When clicked Battle, he will be set free and play.
 */
class MainWorld : public World
{
public:
    MainWorld(int _level = 0);
    ~MainWorld();

    enum WorldStatus
    {
        STATUS_BUILDING = 0,
        STATUS_BATTLE = 1,
        STATUS_NOTIFY_LAUNCH = 2,
        STATUS_CANCEL_BATTLE = 3
    };

    /**
     * Switch from building to battle in the next round
     */
    void notifyLaunch() { status = STATUS_NOTIFY_LAUNCH; }

    /**
     * Cancel battle in the next round
     */
    void cancelBattle() { status = STATUS_CANCEL_BATTLE; }

    void step() override;

private:
    void makeBuildingButtons();
    void makeBattleButtons();

    /**
     * Switch from building to battle
     */
    void launch();

    /**
     * Focus on the ship built by players
     */
    void focus();

    bool loadLevel();

    bool levelCleared() const;

    bool playerKeyDown(int p, int key) override;

    static constexpr float BUILD_W = 30.0f, BUILD_H = 23.0f;
    static constexpr float BATTLE_W = 180.0f, BATTLE_H = 60.0f;

    Rigid *buildFrame, *cancelButton;

    WorldStatus status;

    int level;
    std::unordered_map<int, Enemy*> enemies;

    std::string levelMsg;

    const std::string
        buildingMsg1 =
            "  WELCOME\n"
            "  YOU CAN PICK MATERIALS FROM THE LEFT TO BUILD YOUR SHIP.\n"
            "CLICK ON THE LEFT TO PUT THINGS, ON THE RIGHT TO CANCEL.\n"
            "  WHEN FINISHED, CLICK LAUNCH AND MOVE TO THE RIGHT TO WIN.\n"
            "\n"
            "  ... PRESS ANY KEY TO CONTINUE ...",
        buildingMsg2 =
            "  NOTICE THAT DIFFERENT MATERIALS HAVE DIFFERENT PROPERTIES.\n"
            "  REMEMBER TO PUT ENGINES WITH RED ARROWS TO PROVIDE POWER.\n"
            "  BESIDES, USE THE CROSS THE DELETE THINGS, AND YOU CAN DRAG\n"
            "THINGS WITH YOUR CURSOR.\n"
            "\n"
            "  ... PRESS ANY KEY TO CONTINUE ...";
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

class TestWorldFullView : public MainWorld
{
public:
    TestWorldFullView(int _level);
    void setView(float l, float r, float u, float d) override;
};

#endif // COMPILE_TEST

#endif // WORLD_H_

