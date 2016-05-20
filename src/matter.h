/**
 * Define Several typical matter components
 * Interact with LiquidFun
 */

#ifndef MATTER_H_
#define MATTER_H_

#include <ctime>
#include <cassert>
#include <list>
#include <vector>
#include <Box2D/Box2D.h>
#include "image.h"
#include "const.h"

class World;
class Saver;

/**
 * Base class of all rigid bodies and particles
 */
class Matter
{
public:
    Matter(World *_world) noexcept : world(_world), depth(0.0f), player(0) {}
    virtual ~Matter() noexcept {}

    Matter(const Matter &) = delete;
    Matter &operator=(const Matter &) = delete;

    // choose one of below:
    enum RenderMethod
    {
        RENDER_COLOR = 0,
        RENDER_TEXTURE = 1,
        RENDER_COLOR_WITH_TEXTURE = 2
    };
    virtual RenderMethod getRenderMethod() const { return RENDER_COLOR; }
    // 1. use pure color
    virtual float getColorR() const { assert(false); return 0; }
    virtual float getColorG() const { assert(false); return 0; }
    virtual float getColorB() const { assert(false); return 0; }
    virtual float getColorA() const { assert(false); return 0; }
    // 2. use texture
    virtual ImageName getImage() const { assert(false); return (ImageName)0; }

    void setDepth(float _depth) { depth = _depth; }
    float getDepth() const { return depth; }

    /// Specify whether user can create objects by drawing lines
    /// Can be overidden
    static bool canDrawLine() { return false; }

    /// player = -1 : user, 0 : none, >0 : enemy
    void setPlayer(int _player) { assert(_player >= -1); player = _player; }
    int getPlayer() const { return player; }
    /// mark to be created by users
    void setIsUserCreated(bool b) { setPlayer(b ? -1 : 0); }
    /// determine whether it is created by users
    bool getIsUserCreated() const { return getPlayer() == -1; }

protected:
    World *world;

private:
    float depth;
    int player;
};

/**
 * Base class of all rigid bodies
 */
class Rigid : public Matter
{
public:
    Rigid(World *_world, b2Body *b);
    /// NOTICE: Pointer to b2Shape in fixtureDefs will be deleted
    Rigid(World *_world, const b2BodyDef &bodyDef, const std::vector<b2FixtureDef> &fixtureDefs) noexcept;
    virtual ~Rigid() noexcept;

    b2Body *getReferee() const { return physics; }

    bool testPoint(float x, float y) const;

    void setAlert(AlertType _alert, clock_t expire = 0);
    AlertType getAlert() const;
    void setDefalutAlert(AlertType _alert);
    float getAlertColorR() const;
    float getAlertColorG() const;
    float getAlertColorB() const;
    float getAlertColorA() const;

    /// rigids are undestroyable by default
    virtual float getStrength() const { return INFINITY; }
    /// create a damage effect and delete the rigid
    virtual void damage();

    /// used in putting process when cursor moves
    /// set the object to (x, y)
    virtual bool tryMoveTo(float x, float y, float angle);
    /// used in putting process when cursor clicks
    /// if not overlap with others, set it free, or else display alert
    virtual bool tryPutDown();

    /// bind a key on the keyboard to control it
    /// 0 = none
    virtual void bindKey(int _key) { assert(false); }
    /// what has it bind?
    virtual int getKeyBinded() const { return 0; }
    /// shoud we bind a key to it?
    virtual bool shouldBind() const { return false; }
    /// what should it do when the key above is pressed
    virtual void keyPressed() {}

protected:
    b2Body *physics;

private:
    AlertType alert, defaultAlert;
    clock_t alertExpireClock;
};

/**
 * Base class of all particle systems
 */
class ParticleSystem : public Matter
{
public:
    /// NOTICE: Pointer to b2Shape in groupDefs will be deleted
    ParticleSystem(World *_world, const b2ParticleSystemDef &systemDef, const std::vector<b2ParticleGroupDef> &groupDefs) noexcept;
    virtual ~ParticleSystem() noexcept;

    b2ParticleSystem *getReferee() const { return physics; }

    /// Set to delete it in next round
    static void setDied(ParticleSystem *system) { died.push_back(system); }

    /// Clean all died
    static void cleanDied();

    /// displayed radius = ? * physics radius
    virtual float getDisplayedRadiusScale() const = 0;

protected:
    b2ParticleSystem *physics;

private:
    static std::list<ParticleSystem*> died;
};

/**
 * Base class of all dynamic square rigids
 */
class Block : public Rigid
{
public:
    Block(World *_world, b2Body *b) : Rigid(_world, b) {}
    Block(World *_world, float x, float y, float w, float h, float density, float friction, float restitution) noexcept;
private:
    static b2BodyDef genBodyDef(float x, float y);
    static std::vector<b2FixtureDef> genFixtureDefs(float w, float h, float density, float friction, float restitution);
};

/**
 * Dynamic 1X1 small wooden block
 * Will Float
 */
class SmallWoodBlock : public Block
{
public:
    SmallWoodBlock(World *_world, b2Body *b) : Block(_world, b) {}
    SmallWoodBlock(World *_world, float x, float y, float notused1 = 0, float notused2 = 0) noexcept;

    virtual float getColorR() const override { return WOOD_COLOR_R; }
    virtual float getColorG() const override { return WOOD_COLOR_G; }
    virtual float getColorB() const override { return WOOD_COLOR_B; }
    virtual float getColorA() const override { return WOOD_COLOR_A; }
    virtual float getStrength() const override { return WOOD_STRENGTH; }
};

/**
 * Dynamic 3X3 large wooden block
 * Will Float
 */
class LargeWoodBlock : public Block
{
public:
    LargeWoodBlock(World *_world, b2Body *b) : Block(_world, b) {}
    LargeWoodBlock(World *_world, float x, float y, float notused1 = 0, float notused2 = 0) noexcept;

    virtual float getColorR() const override { return WOOD_COLOR_R; }
    virtual float getColorG() const override { return WOOD_COLOR_G; }
    virtual float getColorB() const override { return WOOD_COLOR_B; }
    virtual float getColorA() const override { return WOOD_COLOR_A; }
    virtual float getStrength() const override { return WOOD_STRENGTH; }
};

/**
 * Base class of engines
 */
class Engine : public Block
{
protected:
    Engine(World *_world, b2Body *b, float _force) : Block(_world, b), force(_force) {}
    Engine(World *_world, float x, float y, float w, float h, float _force) noexcept;

public:
    virtual RenderMethod getRenderMethod() const override { return RENDER_TEXTURE; }
    virtual ImageName getImage() const override { return IMAGE_RED_ARROW; }
    virtual float getColorR() const override { return STEEL_COLOR_R; }
    virtual float getColorG() const override { return STEEL_COLOR_G; }
    virtual float getColorB() const override { return STEEL_COLOR_B; }
    virtual float getColorA() const override { return STEEL_COLOR_A; } // colors are used when damaged
    virtual float getStrength() const override { return STEEL_STRENGTH; }

    void bindKey(int _key) override { key = _key; }
    int getKeyBinded() const override { return key; }
    bool shouldBind() const override { return true; }
    void keyPressed() override;

private:
    friend Saver;
    int key;
    float force;
};

/**
 * 1X2 Small engine that will provide a 30N force
 */
class SmallEngine : public Engine
{
public:
    SmallEngine(World *_world, b2Body *b) : Engine(_world, b, SMALL_ENGINE_FORCE) {}
    SmallEngine(World *_world, float x, float y, float notused1 = 0, float notused2 = 0) noexcept
        : Engine(_world, x, y, 2.0f, 1.0f, SMALL_ENGINE_FORCE)
    {}
};

/**
 * 2X4 Large engine that will provide a 300N force
 */
class LargeEngine : public Engine
{
public:
    LargeEngine(World *_world, b2Body *b) : Engine(_world, b, LARGE_ENGINE_FORCE) {}
    LargeEngine(World *_world, float x, float y, float notused1 = 0, float notused2 = 0) noexcept
        : Engine(_world, x, y, 4.0f, 2.0f, LARGE_ENGINE_FORCE)
    {}
};

/**
 * Steel r=0.5 ball
 */
class SmallSteelBall : public Rigid
{
public:
    SmallSteelBall(World *_world, b2Body *b) : Rigid(_world, b) {}
    SmallSteelBall(World *_world, float x, float y, float notused1 = 0, float notused2 = 0) noexcept;

    virtual float getColorR() const override { return STEEL_COLOR_R; }
    virtual float getColorG() const override { return STEEL_COLOR_G; }
    virtual float getColorB() const override { return STEEL_COLOR_B; }
    virtual float getColorA() const override { return STEEL_COLOR_A; }
    virtual float getStrength() const override { return STEEL_STRENGTH; }
private:
    static b2BodyDef genBodyDef(float x, float y);
    static std::vector<b2FixtureDef> genFixtureDefs();
};

/**
 * r=0.5 round bomb
 */
class Bomb : public Rigid
{
public:
    Bomb(World *_world, b2Body *b) : Rigid(_world, b) {}
    Bomb(World *_world, float x, float y, float notused1 = 0, float notused2 = 0) noexcept;

    virtual float getColorR() const override { return BOMB_COLOR_R; }
    virtual float getColorG() const override { return BOMB_COLOR_G; }
    virtual float getColorB() const override { return BOMB_COLOR_B; }
    virtual float getColorA() const override { return BOMB_COLOR_A; }
    virtual float getStrength() const override { return BOMB_STRENGTH; }
    
    void damage() override;
    void bindKey(int _key) override { key = _key, bindClock = clock(); }
    int getKeyBinded() const override { return key; }
    bool shouldBind() const override { return true; }
    void keyPressed() override { if (clock() - bindClock > CLICK_INTERVAL * CLOCKS_PER_SEC) damage(); }

private:
    static b2BodyDef genBodyDef(float x, float y);
    static std::vector<b2FixtureDef> genFixtureDefs();

    friend Saver;
    int key;
    clock_t bindClock;
};

/**
 * Frame around the world
 * 1 unit thick
 */
class Frame : public Rigid
{
public:
    Frame(World *_world, float l, float r, float d, float u) noexcept;

    virtual float getColorR() const { return FRAME_COLOR_R; }
    virtual float getColorG() const { return FRAME_COLOR_G; }
    virtual float getColorB() const { return FRAME_COLOR_B; }
    virtual float getColorA() const { return FRAME_COLOR_A; }

private:
    static b2BodyDef genBodyDef();
    static std::vector<b2FixtureDef> genFixtureDefs(float l, float r, float d, float u);
};

/**
 * Base class with all that canDrawLine()
 * Will create contact to what is linked
 */
class Stick : public Rigid
{
public:
    Stick(World *_world, b2Body *b) : Rigid(_world, b) {}
    Stick
    (
        World *_world, float _x1, float _y1, float _x2, float _y2, float density, float friction, float restitution
    ) noexcept;

    static bool canDrawLine() { return true; }

    /// Do nothing. There is already a CURSOR_CROSS
    bool tryMoveTo(float x, float y, float angle) override { return true; }
    /// Return false if overlapped with others but not on two ends.
    /// Create joint in two ends.
    bool tryPutDown() override;

protected:
    float x1, y1, x2, y2; // two ends

private:
    static b2BodyDef genBodyDef(float x1, float y1, float x2, float y2);
    static std::vector<b2FixtureDef> genFixtureDefs(
        float x1, float y1, float x2, float y2, float density, float friction, float restitution
    );
};

class SteelStick : public Stick
{
public:
    SteelStick(World *_world, b2Body *b) : Stick(_world, b) {}
    SteelStick(World *_world, float x1, float y1, float x2 = 0, float y2 = 0) noexcept;

    virtual float getColorR() const override { return STEEL_COLOR_R; }
    virtual float getColorG() const override { return STEEL_COLOR_G; }
    virtual float getColorB() const override { return STEEL_COLOR_B; }
    virtual float getColorA() const override { return STEEL_COLOR_A; }
    virtual float getStrength() const override { return STEEL_STRENGTH; }
};

class WoodStick : public Stick
{
public:
    WoodStick(World *_world, b2Body *b) : Stick(_world, b) {}
    WoodStick(World *_world, float x1, float y1, float x2 = 0, float y2 = 0) noexcept;

    virtual float getColorR() const override { return WOOD_COLOR_R; }
    virtual float getColorG() const override { return WOOD_COLOR_G; }
    virtual float getColorB() const override { return WOOD_COLOR_B; }
    virtual float getColorA() const override { return WOOD_COLOR_A; }
    virtual float getStrength() const override { return WOOD_STRENGTH; }
};

template <ImageName imageName>
class Button : public Rigid
{
public:
    Button(World *_world, float l, float r, float d, float u) noexcept;

    virtual RenderMethod getRenderMethod() const override { return RENDER_TEXTURE; }
    virtual ImageName getImage() const override { return imageName; }

private:
    static b2BodyDef genBodyDef(float l, float r, float d, float u);
    static std::vector<b2FixtureDef> genFixtureDefs(float l, float r, float d, float u);
};

/**
 * A square water area
 * This must be use as a pool where the number of particles is static
 */
class WaterSquare : public ParticleSystem
{
public:
    WaterSquare(World *_world, float l, float r, float d, float u) noexcept;

    virtual float getColorR() const override { return (float)WATER_COLOR_R_256/0xFF; }
    virtual float getColorG() const override { return (float)WATER_COLOR_G_256/0xFF; }
    virtual float getColorB() const override { return (float)WATER_COLOR_B_256/0xFF; }
    virtual float getColorA() const override { return (float)WATER_COLOR_A_256/0xFF; }
    virtual float getDisplayedRadiusScale() const override { return WATER_PARTICLE_SIZE_SCALE; }

private:
    static b2ParticleSystemDef genSystemDef();
    static std::vector<b2ParticleGroupDef> genGroupDefs(float l, float r, float d, float u);
};

/**
 * Dust rised when a rigid is damaged
 */
class Dust : public ParticleSystem
{
public:
    /// @param v : linear velocity
    /// @param w : angular velocity
    Dust(
        World *_world, const b2Vec2 &pos, const std::vector<b2Shape*> &shapes,
        const b2Vec2 &v, float w,
        float _colorR, float _colorG, float _colorB, float _colorA
    ) noexcept;

    virtual float getColorR() const override { return colorR; }
    virtual float getColorG() const override { return colorG; }
    virtual float getColorB() const override { return colorB; }
    virtual float getColorA() const override { return colorA; }
    virtual float getDisplayedRadiusScale() const override { return DUST_PARTICLE_SIZE_SCALE; }
private:
    static b2ParticleSystemDef genSystemDef();
    static std::vector<b2ParticleGroupDef> genGroupDefs(
        const b2Vec2 &pos, const std::vector<b2Shape*> &shapes,
        const b2Vec2 &v, float w,
        float _colorR, float _colorG, float _colorB, float _colorA
    );

    float colorR, colorG, colorB, colorA;
};

/**
 * Set when bomb exploses
 */
class Flame : public ParticleSystem
{
public:
    Flame(World *_world, const b2Vec2 &pos, float radius) noexcept;

    virtual float getColorR() const override { return (float)FLAME_COLOR_R_256/0xFF; }
    virtual float getColorG() const override { return (float)FLAME_COLOR_G_256/0xFF; }
    virtual float getColorB() const override { return (float)FLAME_COLOR_B_256/0xFF; }
    virtual float getColorA() const override { return (float)FLAME_COLOR_A_256/0xFF; }
    virtual float getDisplayedRadiusScale() const override { return FLAME_PARTICLE_SIZE_SCALE; }
private:
    static b2ParticleSystemDef genSystemDef();
    static std::vector<b2ParticleGroupDef> genGroupDefs(const b2Vec2 &pos, float radius);
};

// Here we implement template classes

template <ImageName imageName>
inline Button<imageName>::Button(World *_world, float l, float r, float d, float u) noexcept
    : Rigid(_world, genBodyDef(l, r, d, u), genFixtureDefs(l, r, d, u))
{}

template <ImageName imageName>
b2BodyDef Button<imageName>::genBodyDef(float l, float r, float d, float u)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((l + r) / 2, (d + u)/2);
    return bodyDef;
}

template <ImageName imageName>
std::vector<b2FixtureDef> Button<imageName>::genFixtureDefs(float l, float r, float d, float u)
{
    b2PolygonShape *dynamicBox = new b2PolygonShape();
    dynamicBox->SetAsBox(r - l, u - d);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = dynamicBox;
    return { fixtureDef };
}

#endif // MATTER_H_
