#include "world.h"
#include "render.h"
#include "matter.h"

static MyDestructionListener myDestructionListener;

World::World(float leftMost, float rightMost, float downMost, float upMost) noexcept
    : mLeftMost(leftMost), mRightMost(rightMost),
      mDownMost(downMost), mUpMost(upMost),
      mCurLeftMost(leftMost), mCurRightMost(rightMost),
      mCurDownMost(downMost), mCurUpMost(upMost),
      physics(new b2World(b2Vec2(0.0f, -GRAVITY)))
{
    physics->SetDestructionListener(&myDestructionListener);

    new Frame(this, leftMost, rightMost, downMost, upMost);
    // will keep track of it from LiquidFun
}

/**
 * delete Matter objects and then delete physics
 */
World::~World() noexcept
{
    for (b2Body *b = physics->GetBodyList(); b;)
    {
        b2Body *_b = b->GetNext();
        delete (Matter*)b->GetUserData(); // this will delete b2Body too
        b = _b;
    }

    ParticleSystem::cleanDied();
    for (b2ParticleSystem *s = physics->GetParticleSystemList(); s;)
    {
        b2ParticleSystem *_s = s->GetNext();
        assert(s->GetParticleGroupCount() > 0);
        delete (Matter*)(s->GetParticleGroupList()->GetUserData());
        s = _s;
    }

    delete physics;
}

void World::setGLOrtho() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost, 1.0f, -1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void World::drawAll() const noexcept
{
    for (const b2Body *b = physics->GetBodyList(); b; b = b->GetNext())
        Render::getInstance().drawRigid(b);

    for (const b2ParticleSystem *s = physics->GetParticleSystemList(); s; s = s->GetNext())
        Render::getInstance().drawParticleSystem(s);
}

void World::step()
{
    physics->Step
        (
         TIME_STEP,
         VELOCITY_ITERATIONS,
         POSITION_ITERATIONS,
         b2CalculateParticleIterations(GRAVITY, PARTICLE_RADIUS, TIME_STEP)
        );

    ParticleSystem::cleanDied();
}

#ifdef COMPILE_TEST

TestWorldDisplayTriangle::TestWorldDisplayTriangle()
    : World(-10, 10, -10, 10)
{}

void TestWorldDisplayTriangle::drawAll() const noexcept
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -10.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-5.0f, -10.0f, 0.0f);
    glEnd();
}

TestWorldSimplePhysics::TestWorldSimplePhysics()
    : World(-10, 10, -10, 10)
{
    new SmallWoodBlock(this, -5.0f, 5.0f);
    new SmallWoodBlock(this, -4.0f, 8.0f);
    new WaterSquare(this, -10.0f, 10.0f, -10.0f, 0.0f);
}

#endif // COMPILE_TEST
