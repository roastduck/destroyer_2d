#include <cassert>
#include <fstream>
#include <typeinfo>
#include <exception>
#include <stdexcept>
#include "saver.h"
#include "world.h"
#include "matter.h"

template <class T>
void readEnum(std::istream &is, T &x)
{
    int y = 0;
    is >> y;
    x = (T)y;
}

void Saver::saveTo(const World *world, std::vector<std::string> path) noexcept
{
    std::ofstream dataStream(makePath(path));
    int maxId(0);
    tempId.clear();
    
    const b2World *w = world->getReferee();
    for (const b2Body *b = w->GetBodyList(); b; b = b->GetNext())
    {
        if (! ((Matter*)(b->GetUserData()))->getIsUserCreated()) continue;
        dataStream << IO_CONTINUE                           << ' ';
        getBodyData(dataStream, b);
        for (const b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext())
        {
            dataStream << IO_CONTINUE                       << ' ';
            getFixtureData(dataStream, f);
        }
        dataStream << IO_STOP                               << ' ';
        getMatterData(dataStream, (Matter*)(b->GetUserData()));
        tempId[b] = maxId++;
    }
    dataStream << IO_STOP                                   << ' ';
    for (const b2Joint *j = w->GetJointList(); j; j = j->GetNext())
    {
        const b2Body *b1 = (const_cast<b2Joint*>(j))->GetBodyA();
        const b2Body *b2 = (const_cast<b2Joint*>(j))->GetBodyB();
        if (! ((Matter*)(b1->GetUserData()))->getIsUserCreated()) continue;
        if (! ((Matter*)(b2->GetUserData()))->getIsUserCreated()) continue;

        dataStream << IO_CONTINUE                           << ' ';
        getJointData(dataStream, j);
    }
    dataStream << IO_STOP                                   << ' ';
}

bool Saver::loadFrom(World *world, std::vector<std::string> path, float offsetX) noexcept
{
    std::ifstream dataStream(makePath(path));
    if (! dataStream.good()) return false;
    int maxId(0);
    tempAddr.clear();
    
    try
    {
        b2World *w = world->getReferee();
        while (getFlag(dataStream) == IO_CONTINUE)
        {
            b2Body *b = setBodyData(dataStream, w, offsetX);
            while (getFlag(dataStream) == IO_CONTINUE)
                setFixtureData(dataStream, b);
            setMatterData(dataStream, world, b);
            tempAddr[maxId++] = b;
        }
        while (getFlag(dataStream) == IO_CONTINUE)
            setJointData(dataStream, w);
    } catch (const std::exception &e)
    {
        std::cout << "Error occurred when loading savings : " << e.what() << std::endl;
        return false;
    }

    return true;
}

#ifndef ROOT_DIR
    #error Macro ROOT_DIR not found
#endif

#ifdef _WIN32
    #define SLASH "\\"
#else
    #define SLASH "/"
#endif

std::string Saver::makePath(std::vector<std::string> path)
{
    std::string ret = ROOT_DIR;
    for (const std::string &seg : path)
        ret += SLASH + seg;
    return ret;
}

#undef SLASH

Saver::MatterId Saver::getMatterId(const Matter *m)
{
    const auto &tid = typeid(*m);
    if (tid == typeid(SmallWoodBlock)) return ID_SMALL_WOOD_BLOCK;
    if (tid == typeid(LargeWoodBlock)) return ID_LARGE_WOOD_BLOCK;
    if (tid == typeid(SmallEngine)) return ID_SMALL_ENGINE;
    if (tid == typeid(LargeEngine)) return ID_LARGE_ENGINE;
    if (tid == typeid(SmallSteelBall)) return ID_SMALL_STEEL_BALL;
    if (tid == typeid(Bomb)) return ID_BOMB;
    if (tid == typeid(SteelStick)) return ID_STEEL_STICK;
    if (tid == typeid(WoodStick)) return ID_WOOD_STICK;
    assert(false);
}

Saver::IOFlag Saver::getFlag(std::istream &is)
{
    IOFlag x;
    readEnum(is, x);
    if (x == IO_CONTINUE) return IO_CONTINUE;
    if (x == IO_STOP) return IO_STOP;
    throw std::runtime_error("File broken");
}

void Saver::getBodyData(std::ostream &os, const b2Body *b)
{
    os << b->GetType()                                      << ' '
       << b->GetPosition().x                                << ' '
       << b->GetPosition().y                                << ' '
       << b->GetAngle()                                     << ' '
       << b->IsBullet()                                     << ' ';
}

b2Body *Saver::setBodyData(std::istream &is, b2World *w, float offsetX)
{
    b2BodyDef def;
    readEnum(is, def.type);
    is >> def.position.x
       >> def.position.y
       >> def.angle
       >> def.bullet;
    def.position.x += offsetX;
    return w->CreateBody(&def);
}

void Saver::getFixtureData(std::ostream &os, const b2Fixture *f)
{
    os << f->GetFriction()                                  << ' '
       << f->GetRestitution()                               << ' '
       << f->GetDensity()                                   << ' '
       << f->GetType()                                      << ' ';
    switch (f->GetType())
    {
    case b2Shape::e_circle:
        {
            b2CircleShape *s = (b2CircleShape*)(f->GetShape());
            os << s->m_radius                               << ' '
               << s->m_p.x                                  << ' '
               << s->m_p.y                                  << ' ';
        }
        break;
    case b2Shape::e_polygon:
        {
            b2PolygonShape *s = (b2PolygonShape*)(f->GetShape());
            os << s->m_count                                << ' ';
            for (int i = 0; i < s->m_count; i++)
                os << s->m_vertices[i].x                    << ' '
                   << s->m_vertices[i].y                    << ' ';
        }
        break;
    default:
        assert(false);
    }
}

b2Fixture *Saver::setFixtureData(std::istream &is, b2Body *b)
{
    b2FixtureDef def;
    is >> def.friction
       >> def.restitution
       >> def.density;
    b2Shape::Type type;
    readEnum(is, type);
    switch (type)
    {
    case b2Shape::e_circle:
        {
            b2CircleShape *s = new b2CircleShape();
            is >> s->m_radius
               >> s->m_p.x
               >> s->m_p.y;
            def.shape = s;
        }
        break;
    case b2Shape::e_polygon:
        {
            b2PolygonShape *s = new b2PolygonShape();
            int _cnt;
            is >> _cnt;
            b2Vec2 *_vert = new b2Vec2[_cnt];
            for (int i = 0; i < _cnt; i++)
                is >> _vert[i].x
                   >> _vert[i].y;
            s->Set(_vert, _cnt);
            delete [] _vert;
            def.shape = s;
        }
        break;
    default:
        assert(false);
    }
    b2Fixture *f = b->CreateFixture(&def);
    delete def.shape;
    return f;
}

void Saver::getJointData(std::ostream &os, const b2Joint *j)
{
    const b2Body *b1 = (const_cast<b2Joint*>(j))->GetBodyA();
    const b2Body *b2 = (const_cast<b2Joint*>(j))->GetBodyB();

    os << j->GetType()                                      << ' ';
    switch (j->GetType())
    {
    case e_revoluteJoint:
        {
            const b2RevoluteJoint *r = (const b2RevoluteJoint*)j;
            assert(tempId.count(b1));
            assert(tempId.count(b2));
            os << tempId[b1] << ' '
               << tempId[b2] << ' '
               << r->GetLocalAnchorA().x                    << ' '
               << r->GetLocalAnchorA().y                    << ' '
               << r->GetLocalAnchorB().x                    << ' '
               << r->GetLocalAnchorB().y                    << ' ';
        }
        break;
    default:
        break; // not saving
    }
}

b2Joint *Saver::setJointData(std::istream &is, b2World *w)
{
    b2JointType type;
    readEnum(is, type);
    b2Joint *j = NULL;
    switch (type)
    {
    case e_revoluteJoint:
        {
            int idA, idB;
            b2RevoluteJointDef def;
            is >> idA
               >> idB;
            def.bodyA = tempAddr[idA];
            def.bodyB = tempAddr[idB];
            is >> def.localAnchorA.x
               >> def.localAnchorA.y
               >> def.localAnchorB.x
               >> def.localAnchorB.y;
            j = w->CreateJoint(&def);
        }
        break;
    default:
        assert(false);
    }
    return j;
}

void Saver::getMatterData(std::ostream &os, const Matter *m)
{
    MatterId id = getMatterId(m);
    os << id                                                << ' ';
    switch (id)
    {
    case ID_BOMB:
        os << ((Bomb*)m)->key                               << ' ';
        break;
    case ID_SMALL_ENGINE:
    case ID_LARGE_ENGINE:
        os << ((Engine*)m)->key                             << ' ';
        break;
    default:
        break;
    }
}

Matter *Saver::setMatterData(std::istream &is, World *world, b2Body *b)
{
    MatterId id;
    readEnum(is, id);
    Matter *m;
    switch (id)
    {
    case ID_SMALL_WOOD_BLOCK:
        m = new SmallWoodBlock(world, b);
        break;
    case ID_LARGE_WOOD_BLOCK:
        m = new LargeWoodBlock(world, b);
        break;
    case ID_SMALL_ENGINE:
        m = new SmallEngine(world, b);
        is >> ((Engine*)m)->key;
        break;
    case ID_LARGE_ENGINE:
        m = new LargeEngine(world, b);
        is >> ((Engine*)m)->key;
        break;
    case ID_SMALL_STEEL_BALL:
        m = new SmallSteelBall(world, b);
        break;
    case ID_BOMB:
        m = new Bomb(world, b);
        is >> ((Bomb*)m)->key;
        ((Bomb*)m)->bindClock = clock();
        break;
    case ID_STEEL_STICK:
        m = new SteelStick(world, b);
        break;
    case ID_WOOD_STICK:
        m = new WoodStick(world, b);
        break;
    }
    m->setIsUserCreated(true);
    return m;
}

