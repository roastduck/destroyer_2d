/**
 * Define Several typical matter components
 * Interact with LiquidFun
 */

#ifndef MATTER_H_
#define MATTER_H_

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
    Matter(World *_world) : world(_world) {}
    virtual ~Matter() {}

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
    Rigid(World *_world, const b2BodyDef &bodyDef, const std::vector<b2FixtureDef> &fixtureDefs);
    virtual ~Rigid();

protected:
    b2Body *physics;
};

/**
 * Dynamic 2X2 small wooden block
 * Will Float
 */
class SmallWoodBlock : public Rigid
{
public:
    SmallWoodBlock(World *_world, float x, float y);

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
    Frame(World *_world, float l, float r, float d, float u);

    virtual float getColorR() const { return FRAME_COLOR_R; }
    virtual float getColorG() const { return FRAME_COLOR_G; }
    virtual float getColorB() const { return FRAME_COLOR_B; }
    virtual float getColorA() const { return FRAME_COLOR_A; }

private:
    static b2BodyDef genBodyDef();
    static std::vector<b2FixtureDef> genFixtureDefs(float l, float r, float d, float u);
};

#endif // MATTER_H_
