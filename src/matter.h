/**
 * Define Several typical matter components
 * Interact with LiquidFun
 */

#ifndef MATTER_H_
#define MATTER_H_

#include <list>
#include <vector>
#include <Box2D/Box2D.h>
#include "world.h"
#include "const.h"

/**
 * Base class of all rigid bodies and particles
 */
class Matter
{
public:
    Matter(World *_world) noexcept : world(_world) {}
    virtual ~Matter() noexcept {}

    virtual float getColorR() const = 0;
    virtual float getColorG() const = 0;
    virtual float getColorB() const = 0;
    virtual float getColorA() const = 0;

protected:
    World *world;
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

    void setAlert(AlertType _alert) { alert = _alert; }
    AlertType getAlert() const { return alert; }
    float getAlertColorR() const;
    float getAlertColorG() const;
    float getAlertColorB() const;
    float getAlertColorA() const;

protected:
    b2Body *physics;

private:
    AlertType alert;
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
    SmallWoodBlock(World *_world, float x, float y) noexcept;

    virtual float getColorR() const { return WOOD_COLOR_R; }
    virtual float getColorG() const { return WOOD_COLOR_G; }
    virtual float getColorB() const { return WOOD_COLOR_B; }
    virtual float getColorA() const { return WOOD_COLOR_A; }

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
