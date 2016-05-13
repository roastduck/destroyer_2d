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

/**
 * Base class of all rigid bodies and particles
 */
class Matter
{
public:
    Matter(World *_world) noexcept : world(_world), depth(0.0f) {}
    virtual ~Matter() noexcept {}

    // choose one of below:
    enum RenderMethod
    {
        RENDER_COLOR = 0,
        RENDER_TEXTURE = 1
    };
    virtual RenderMethod getRenderMethod() const { return RENDER_COLOR; }
    // 1. use pure color
    virtual float getColorR() const { assert(false); }
    virtual float getColorG() const { assert(false); }
    virtual float getColorB() const { assert(false); }
    virtual float getColorA() const { assert(false); }
    // 2. use texture
    virtual ImageName getImage() const { assert(false); }

    void setDepth(float _depth) { depth = _depth; }
    float getDepth() const { return depth; }

    /// Specify whether user can create objects by drawing lines
    /// Can be overidden
    static bool canDrawLine() { return false; }

protected:
    World *world;

private:
    float depth;
};

/**
 * Base class of all rigid bodies
 */
class Rigid : public Matter
{
public:
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

    /// used in putting process when cursor moves
    /// set the object to (x, y)
    virtual bool tryMoveTo(float x, float y, float angle);
    /// used in putting process when cursor clicks
    /// if not overlap with others, set it free, or else display alert
    virtual bool tryPutDown();

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
    static void cleanDied() { for (ParticleSystem *s : died) delete s; died.clear(); }

protected:
    b2ParticleSystem *physics;

private:
    static std::list<ParticleSystem*> died;
};

/**
 * Dynamic 2X2 small wooden block
 * Will Float
 */
class SmallWoodBlock : public Rigid
{
public:
    SmallWoodBlock(World *_world, float x, float y, float notused1 = 0, float notused2 = 0) noexcept;

    virtual float getColorR() const override { return WOOD_COLOR_R; }
    virtual float getColorG() const override { return WOOD_COLOR_G; }
    virtual float getColorB() const override { return WOOD_COLOR_B; }
    virtual float getColorA() const override { return WOOD_COLOR_A; }
private:
    static b2BodyDef genBodyDef(float x, float y);
    static std::vector<b2FixtureDef> genFixtureDefs();
};

/**
 * Frame around the world
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
    Stick
    (
        World *_world, const b2BodyDef &bodyDef, const std::vector<b2FixtureDef> &fixtureDefs, float _x1, float _y1, float _x2, float _y2
    ) noexcept;

    static bool canDrawLine() { return true; }

    /// Do nothing. There is already a CURSOR_CROSS
    bool tryMoveTo(float x, float y, float angle) override { return true; }
    /// Return false if overlapped with others but not on two ends.
    /// Create joint in two ends.
    bool tryPutDown() override;

protected:
    float x1, y1, x2, y2; // two ends
};

class SteelStick : public Stick
{
public:
    SteelStick(World *_world, float x1, float y1, float x2 = 0, float y2 = 0) noexcept;

    virtual float getColorR() const override { return STEEL_COLOR_R; }
    virtual float getColorG() const override { return STEEL_COLOR_G; }
    virtual float getColorB() const override { return STEEL_COLOR_B; }
    virtual float getColorA() const override { return STEEL_COLOR_A; }
private:
    static b2BodyDef genBodyDef(float x1, float y1, float x2, float y2);
    static std::vector<b2FixtureDef> genFixtureDefs(float x1, float y1, float x2, float y2);
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

    virtual float getColorR() const { return (float)WATER_COLOR_R_256/0xFF; }
    virtual float getColorG() const { return (float)WATER_COLOR_G_256/0xFF; }
    virtual float getColorB() const { return (float)WATER_COLOR_B_256/0xFF; }
    virtual float getColorA() const { return (float)WATER_COLOR_A_256/0xFF; }

private:
    static b2ParticleSystemDef genSystemDef();
    static std::vector<b2ParticleGroupDef> genGroupDefs(float l, float r, float d, float u);
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
