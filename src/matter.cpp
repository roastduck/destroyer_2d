#include <cstdlib>
#include <cassert>
#include "world.h"
#include "matter.h"

static float random(float l, float r) { return (float)rand() / RAND_MAX * (r-l) + l; }

std::list<ParticleSystem*> ParticleSystem::died;

Rigid::Rigid(World *_world, b2Body *b)
    : Matter(_world), physics(b), alert(ALERT_NONE), defaultAlert(ALERT_NONE), alertExpireClock(0)
{
    if (physics) physics->SetUserData(this);
}

Rigid::Rigid(World *_world, const b2BodyDef &bodyDef, const std::vector<b2FixtureDef> &fixtureDefs) noexcept
    : Rigid(_world, NULL)
{
    physics = world->getReferee()->CreateBody(&bodyDef);
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
    world->getReferee()->DestroyBody(physics);
}

void Rigid::setAlert(AlertType _alert, clock_t expire)
{
    alert = _alert;
    alertExpireClock = expire ? clock() + expire : 0;
}

AlertType Rigid::getAlert() const
{
    if (! alertExpireClock || alertExpireClock > clock())
        return alert;
    else
        return defaultAlert;
}

void Rigid::setDefalutAlert(AlertType _alert)
{
    if (alert == defaultAlert)
        alert = _alert;
    defaultAlert = _alert;
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
    switch (getAlert())
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_R;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_R;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_R;
    case ALERT_SHADOW:
        return ALERT_SHADOW_COLOR_R;
    default:
        assert(false);
    }
}

float Rigid::getAlertColorG() const
{
    switch (getAlert())
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_G;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_G;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_G;
    case ALERT_SHADOW:
        return ALERT_SHADOW_COLOR_G;
    default:
        assert(false);
    }
}

float Rigid::getAlertColorB() const
{
    switch (getAlert())
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_B;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_B;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_B;
    case ALERT_SHADOW:
        return ALERT_SHADOW_COLOR_B;
    default:
        assert(false);
    }
}

float Rigid::getAlertColorA() const
{
    switch (getAlert())
    {
    case ALERT_NORMAL:
        return ALERT_NORMAL_COLOR_A;
    case ALERT_HOVER:
        return ALERT_HOVER_COLOR_A;
    case ALERT_WARNING:
        return ALERT_WARNING_COLOR_A;
    case ALERT_SHADOW:
        return ALERT_SHADOW_COLOR_A;
    default:
        assert(false);
    }
}

void Rigid::damage()
{
    std::vector<b2Shape*> shapes; // should clone
    for (const b2Fixture *f = physics->GetFixtureList(); f; f = f->GetNext())
    {
        b2Shape *p;
        switch (f->GetShape()->GetType())
        {
        case b2Shape::e_circle:
            p = new b2CircleShape(*((b2CircleShape*)(f->GetShape())));
            break;
        case b2Shape::e_polygon:
            p = new b2PolygonShape(*((b2PolygonShape*)(f->GetShape())));
            break;
        default:
            assert(false);
        }
        shapes.push_back(p);
    }
    new Dust(
        world, physics->GetPosition(), shapes,
        physics->GetLinearVelocity(), physics->GetAngularVelocity(),
        getColorR(), getColorG(), getColorB(), getColorA()
    );

    delete this;
}

bool Rigid::tryMoveTo(float x, float y, float angle)
{
    physics->SetTransform(b2Vec2(x, y), angle);
    return true;
}

bool Rigid::tryPutDown()
{
    for (b2Body *b = world->getReferee()->GetBodyList(); b; b = b->GetNext())
        if (b != physics)
            for (b2Fixture *f1 = b->GetFixtureList(); f1; f1 = f1->GetNext())
                for (b2Fixture *f2 = physics->GetFixtureList(); f2; f2 = f2->GetNext())
                    if (b2TestOverlap(f1->GetShape(), 0, f2->GetShape(), 0, b->GetTransform(), physics->GetTransform()))
                        return false;
    
    physics->SetType(b2_dynamicBody);
    physics->SetActive(true);
    setDepth(0.0f);
    setDefalutAlert(ALERT_NONE);
    return true;
}

ParticleSystem::ParticleSystem(World *_world, const b2ParticleSystemDef &systemDef, const std::vector<b2ParticleGroupDef> &groupDefs) noexcept
    : Matter(_world)
{
    physics = world->getReferee()->CreateParticleSystem(&systemDef);
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
    world->getReferee()->DestroyParticleSystem(physics);
    for (auto i = died.begin(); i != died.end();)
    {
        auto _i(i);
        _i ++;
        if (*i == this)
            died.erase(i);
        i = _i;
    }
}

void ParticleSystem::cleanDied()
{
    for (auto i = died.begin(); i != died.end();)
    {
        auto _i = i;
        _i ++;
        delete *i;
        i = _i;
    }
    // died will be cleared in destructor
}

Block::Block(World *_world, float x, float y, float w, float h, float density, float friction, float restitution) noexcept
    : Rigid(_world, genBodyDef(x, y), genFixtureDefs(w, h, density, friction, restitution))
{}

b2BodyDef Block::genBodyDef(float x, float y)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    return bodyDef;
}

std::vector<b2FixtureDef> Block::genFixtureDefs(float w, float h, float density, float friction, float restitution)
{
    b2PolygonShape *dynamicBox = new b2PolygonShape();
    dynamicBox->SetAsBox(w / 2, h / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    return { fixtureDef };
}

SmallWoodBlock::SmallWoodBlock(World *_world, float x, float y, float, float) noexcept
    : Block(_world, x, y, 1.0f, 1.0f, WOOD_DENSITY, WOOD_FRICTION, WOOD_RESTITUTION)
{}

LargeWoodBlock::LargeWoodBlock(World *_world, float x, float y, float, float) noexcept
    : Block(_world, x, y, 3.0f, 3.0f, WOOD_DENSITY, WOOD_FRICTION, WOOD_RESTITUTION)
{}

Engine::Engine(World *_world, float x, float y, float w, float h, float _force) noexcept
    : Block(_world, x, y, w, h, STEEL_DENSITY, STEEL_FRICTION, STEEL_RESTITUTION), key(0), force(_force)
{}

void Engine::keyPressed()
{
    physics->ApplyForceToCenter(physics->GetWorldVector(b2Vec2(force, 0.0f)), true);
}

SmallSteelBall::SmallSteelBall(World *_world, float x, float y, float, float) noexcept
    : Rigid(_world, genBodyDef(x, y), genFixtureDefs())
{}

b2BodyDef SmallSteelBall::genBodyDef(float x, float y)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    return bodyDef;
}

std::vector<b2FixtureDef> SmallSteelBall::genFixtureDefs()
{
    b2CircleShape *dynamicBall = new b2CircleShape();
    dynamicBall->m_radius = 0.5f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = dynamicBall;
    fixtureDef.density = STEEL_DENSITY;
    fixtureDef.friction = STEEL_FRICTION;
    fixtureDef.restitution = STEEL_RESTITUTION;
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

Stick::Stick
(
    World *_world, float _x1, float _y1, float _x2, float _y2, float density, float friction, float restitution
) noexcept
    : Rigid(_world, genBodyDef(_x1, _y1, _x2, _y2), genFixtureDefs(_x1, _y1, _x2, _y2, density, friction, restitution)),
      x1(_x1), y1(_y1), x2(_x2), y2(_y2)
{}

bool Stick::tryPutDown()
{
    for (b2Body *b = world->getReferee()->GetBodyList(); b; b = b->GetNext())
        if (b != physics)
        {
            bool overlap(false);
            b2Fixture *end2Fix = physics->GetFixtureList()/*tail*/, *end1Fix = end2Fix->GetNext(), *mainFix = end1Fix->GetNext();
            int end(0); // 0=bad, 1=to (x1,y1), 2=to (x2,y2), 3=to (x1,y1) and (x2,y2)
            for (b2Fixture *f1 = b->GetFixtureList(); f1 && !overlap; f1 = f1->GetNext())
            {
                if (b2TestOverlap(f1->GetShape(), 0, mainFix->GetShape(), 0, b->GetTransform(), physics->GetTransform()))
                    overlap = true;
                if (b2TestOverlap(f1->GetShape(), 0, end1Fix->GetShape(), 0, b->GetTransform(), physics->GetTransform()))
                    overlap = true, end |= 1;
                if (b2TestOverlap(f1->GetShape(), 0, end2Fix->GetShape(), 0, b->GetTransform(), physics->GetTransform()))
                    overlap = true, end |= 2;
            }
            if (! overlap) continue;
            if (! end) return false;

            b2RevoluteJointDef jointDef;
            if (end & 1)
            {
                jointDef.Initialize(b, physics, b2Vec2(x1, y1));
                world->getReferee()->CreateJoint(&jointDef);
            }
            if (end & 2)
            {
                jointDef.Initialize(b, physics, b2Vec2(x2, y2));
                world->getReferee()->CreateJoint(&jointDef);
            }
        }
    return true;
}

b2BodyDef Stick::genBodyDef(float x1, float y1, float x2, float y2)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set((x1 + x2) / 2, (y1 + y2) / 2);
    bodyDef.bullet = true;
    return bodyDef;
}

std::vector<b2FixtureDef> Stick::genFixtureDefs(
    float x1, float y1, float x2, float y2, float density, float friction, float restitution
)
{
    b2PolygonShape *mainBox = new b2PolygonShape(),
                   *end1Box = new b2PolygonShape(), *end2Box = new b2PolygonShape();
    float length = sqrtf((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
    float angle = atan2f(y2 - y1, x2 - x1);
    mainBox->SetAsBox(length / 2 - STICK_END_THICKNESS / 2, STICK_THICKNESS / 2, b2Vec2(0.0f, 0.0f), angle);
    end1Box->SetAsBox(STICK_END_THICKNESS / 2, STICK_END_THICKNESS / 2, b2Vec2((x1-x2) / 2, (y1-y2) / 2), angle);
    end2Box->SetAsBox(STICK_END_THICKNESS / 2, STICK_END_THICKNESS / 2, b2Vec2((x2-x1) / 2, (y2-y1) / 2), angle);
    b2FixtureDef mainDef, end1Def, end2Def;
    mainDef.density = end1Def.density = end2Def.density = density;
    mainDef.friction = end1Def.friction = end2Def.friction = friction;
    mainDef.restitution = end1Def.restitution = end2Def.restitution = restitution;
    mainDef.shape = mainBox, end1Def.shape = end1Box, end2Def.shape = end2Box;
    return { mainDef, end1Def, end2Def };
}

SteelStick::SteelStick(World *_world, float x1, float y1, float x2, float y2) noexcept
    : Stick(_world, x1, y1, x2, y2, STEEL_DENSITY, STEEL_FRICTION, STEEL_RESTITUTION)
{}

WoodStick::WoodStick(World *_world, float x1, float y1, float x2, float y2) noexcept
    : Stick(_world, x1, y1, x2, y2, WOOD_DENSITY, WOOD_FRICTION, WOOD_RESTITUTION)
{}

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

Dust::Dust(
    World *_world, const b2Vec2 &pos, const std::vector<b2Shape*> &shapes,
    const b2Vec2 &v, float w,
    float _colorR, float _colorG, float _colorB, float _colorA
) noexcept
    : ParticleSystem(_world, genSystemDef(), genGroupDefs(pos, shapes, v, w, _colorR, _colorG, _colorB, _colorA)),
      colorR(_colorR), colorG(_colorG), colorB(_colorB), colorA(_colorA)
{
    for (int i = 0; i < physics->GetParticleCount(); i++)
    {
        physics->SetParticleLifetime(i, random(0, 3));
        physics->GetPositionBuffer()[i].x += random(-0.1f, 0.1f);
        physics->GetPositionBuffer()[i].y += random(-0.1f, 0.1f);
        physics->GetVelocityBuffer()[i].x += random(-0.7f, 0.7f);
        physics->GetVelocityBuffer()[i].y += random(-0.7f, 0.7f);
    }
}

b2ParticleSystemDef Dust::genSystemDef()
{
    b2ParticleSystemDef systemDef;
    systemDef.radius = PARTICLE_RADIUS;
    systemDef.destroyByAge = true;
    systemDef.gravityScale = 0.0f;
    return systemDef;
}

std::vector<b2ParticleGroupDef> Dust::genGroupDefs(
    const b2Vec2 &pos, const std::vector<b2Shape*> &shapes,
    const b2Vec2 &v, float w,
    float _colorR, float _colorG, float _colorB, float _colorA
)
{
    std::vector<b2ParticleGroupDef> ret(shapes.size());
    for (size_t i = 0; i < ret.size(); i++)
    {
        ret[i].shape = shapes[i];
        ret[i].flags = b2_powderParticle;
        ret[i].position = pos;
        ret[i].linearVelocity = v;
        ret[i].angularVelocity = w;
        ret[i].color.Set(_colorR * 0xFF, _colorG * 0xFF, _colorB * 0xFF, _colorA * 0xFF);
    }
    return ret;
}

