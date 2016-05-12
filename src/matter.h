/**
 * Define Several typical matter components
 * Interact with LiquidFun
 */

#ifndef MATTER_H_
#define MATTER_H_

#include <list>
#include <ctime>
#include <vector>
#include <Box2D/Box2D.h>
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

    virtual float getColorR() const = 0;
    virtual float getColorG() const = 0;
    virtual float getColorB() const = 0;
    virtual float getColorA() const = 0;

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

    // These two functions used in putting process
    bool tryMoveTo(float x, float y, float angle);
    bool tryPutDown();

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
        World *_world, const b2BodyDef &bodyDef, const std::vector<b2FixtureDef> &fixtureDefs, float x1, float y1, float x2, float y2
    ) noexcept;

    static bool canDrawLine() { return true; }
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

#endif // MATTER_H_
