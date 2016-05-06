#include <cassert>
#include "matter.h"

Rigid::Rigid(World *_world, const b2BodyDef &bodyDef, const std::vector<b2FixtureDef> &fixtureDefs)
    : Matter(_world)
{
    physics = world->getB2World()->CreateBody(&bodyDef);
    physics->SetUserData(this);
    for (const b2FixtureDef &fixtureDef : fixtureDefs)
    {
        physics->CreateFixture(&fixtureDef);
        assert(fixtureDef.shape != NULL);
        delete fixtureDef.shape;
    }
}

Rigid::~Rigid()
{
    world->getB2World()->DestroyBody(physics);
}

SmallWoodBlock::SmallWoodBlock(World *_world, float x, float y)
    : Rigid(_world, genBodyDef(x, y), genFixtureDefs())
{}

b2BodyDef SmallWoodBlock::genBodyDef(float x, float y)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    return bodyDef;
}

std::vector<b2FixtureDef> SmallWoodBlock::genFixtureDefs()
{
    b2PolygonShape *dynamicBox = new b2PolygonShape();
    dynamicBox->SetAsBox(1.0f, 1.0f); // 2X2
    b2FixtureDef fixtureDef;
    fixtureDef.shape = dynamicBox;
    fixtureDef.density = 0.4f;
    fixtureDef.friction = 0.7f;
    return { fixtureDef };
}

Frame::Frame(World *_world, float l, float r, float d, float u)
    : Rigid(_world, genBodyDef(), genFixtureDefs(l, r, d, u))
{}

b2BodyDef Frame::genBodyDef()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0.0f, 0.0f);
    return bodyDef;
}

std::vector<b2FixtureDef> Frame::genFixtureDefs(float l, float r, float d, float u)
{
    b2PolygonShape
        *boxL = new b2PolygonShape(),
        *boxR = new b2PolygonShape(),
        *boxD = new b2PolygonShape(),
        *boxU = new b2PolygonShape();
    boxL->SetAsBox(0.5f, (u-d)*0.5f, b2Vec2(l-0.5f, (u+d)*0.5f), 0.0f);
    boxR->SetAsBox(0.5f, (u-d)*0.5f, b2Vec2(r+0.5f, (u+d)*0.5f), 0.0f);
    boxD->SetAsBox((r-l)*0.5f, 0.5f, b2Vec2((l+r)*0.5f, d-0.5f), 0.0f);
    boxU->SetAsBox((r-l)*0.5f, 0.5f, b2Vec2((l+r)*0.5f, u+0.5f), 0.0f);
    b2FixtureDef defL, defR, defD, defU;
    defL.shape = boxL;
    defR.shape = boxR;
    defD.shape = boxD;
    defU.shape = boxU;
    return { defL, defR, defD, defU };
}

