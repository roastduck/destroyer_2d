#include "world.h"
#include "render.h"
#include "matter.h"
#include "mousecallback.h"

static MyDestructionListener myDestructionListener;

World::World(float leftMost, float rightMost, float downMost, float upMost) noexcept
    : mWindow(NULL), mMouseHandler(new MouseHandler(this)),
      mLeftMost(leftMost), mRightMost(rightMost),
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
    assert(mMouseHandler != NULL);
    delete mMouseHandler;

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

    assert(physics != NULL);
    delete physics;
}

void World::setGLOrtho() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost, 100.0f, -100.0f);
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
    mMouseHandler->process();

    drawAll();

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

TestWorldButtons::TestWorldButtons()
    : World(-10, 10, -10, 10)
{
    new WaterSquare(this, -10.0f, 10.0f, -10.0f, -8.0f);

    callback1 = new NewObjectCallback<SmallWoodBlock>(mMouseHandler);
    SmallWoodBlock *button1 = new SmallWoodBlock(this, -7.0f, 7.0f);
    button1->getReferee()->SetType(b2_staticBody);
    mMouseHandler->addButton(button1, callback1);

    callback2 = new NewObjectCallback<SteelStick>(mMouseHandler);
    SteelStick *button2 = new SteelStick(this, -6.0f, 3.0f, -8.0f, 5.0f);
    button2->getReferee()->SetType(b2_staticBody);
    mMouseHandler->addButton(button2, callback2);
}

TestWorldButtons::~TestWorldButtons()
{
    delete callback1;
    delete callback2;
}

#endif // COMPILE_TEST
