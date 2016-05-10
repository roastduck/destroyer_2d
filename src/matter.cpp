#include <cassert>
#include "matter.h"

std::list<ParticleSystem*> ParticleSystem::died;

void MyDestructionListener::SayGoodbye(b2ParticleGroup *group)
{
    b2ParticleSystem *system = group->GetParticleSystem();
    assert(system->GetParticleGroupCount() > 0);
    if (system->GetParticleGroupCount() == 1)
        ParticleSystem::setDied((ParticleSystem*)(system->GetParticleGroupList()->GetUserData()));
}

Rigid::Rigid(World *_world, const b2BodyDef &bodyDef, const std::vector<b2FixtureDef> &fixtureDefs) noexcept
    : Matter(_world), alert(ALERT_NONE)
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

Rigid::~Rigid() noexcept
{
    world->getB2World()->DestroyBody(physics);
}

bool Rigid::testPoint(float x, float y) const
{
    for (b2Fixture *f = physics->GetFixtureList(); f; f = f->GetNext())
        if (f->TestPoint(b2Vec2(x, y)))
            return true;
    return false;
}

float Rigid::getAlertColorR() const
{
    switch (alert)
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_R;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_R;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_R;
    default:
        assert(false);
    }
}

float Rigid::getAlertColorG() const
{
    switch (alert)
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_G;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_G;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_G;
    default:
        assert(false);
    }
}

float Rigid::getAlertColorB() const
{
    switch (alert)
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_B;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_B;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_B;
    default:
        assert(false);
    }
}

float Rigid::getAlertColorA() const
{
    switch (alert)
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_A;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_A;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_A;
    default:
        assert(false);
    }
}

ParticleSystem::ParticleSystem(World *_world, const b2ParticleSystemDef &systemDef, const std::vector<b2ParticleGroupDef> &groupDefs) noexcept
    : Matter(_world)
{
    physics = world->getB2World()->CreateParticleSystem(&systemDef);
    for (const b2ParticleGroupDef &groupDef : groupDefs)
    {
        b2ParticleGroup *group = physics->CreateParticleGroup(groupDef);
        group->SetUserData(this);
        assert(groupDef.shape != NULL);
        delete groupDef.shape;
    }
}

ParticleSystem::~ParticleSystem() noexcept
{
    world->getB2World()->DestroyParticleSystem(physics);
    for (auto i = died.begin(); i != died.end();)
    {
        auto _i(i);
        _i ++;
        if (*i == this)
            died.erase(i);
        i = _i;
    }
}

SmallWoodBlock::SmallWoodBlock(World *_world, float x, float y) noexcept
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

Frame::Frame(World *_world, float l, float r, float d, float u) noexcept
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

WaterSquare::WaterSquare(World *_world, float l, float r, float d, float u) noexcept
    : ParticleSystem(_world, genSystemDef(), genGroupDefs(l, r, d, u))
{}

b2ParticleSystemDef WaterSquare::genSystemDef()
{
    b2ParticleSystemDef systemDef;
    systemDef.radius = PARTICLE_RADIUS;
    systemDef.destroyByAge = false;
    return systemDef;
}

std::vector<b2ParticleGroupDef> WaterSquare::genGroupDefs(float l, float r, float d, float u)
{
    b2PolygonShape *box = new b2PolygonShape();
    box->SetAsBox((r-l)*0.5f, (u-d)*0.5f);
    b2ParticleGroupDef groupDef;
    groupDef.shape = box;
    groupDef.flags = b2_waterParticle;
    groupDef.position.Set((l+r)*0.5f, (d+u)*0.5f);
    groupDef.color.Set(WATER_COLOR_R_256, WATER_COLOR_G_256, WATER_COLOR_B_256, WATER_COLOR_A_256);
    return { groupDef };
}

