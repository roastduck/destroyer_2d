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

void World::setView(float l, float r, float d, float u)
{
    mCurLeftMost = l;
    mCurRightMost = r;
    mCurDownMost = d;
    mCurUpMost = u;
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

MainWorld::MainWorld()
    : World(0, BATTLE_W, 0, BATTLE_H)
{
    float padding = (BATTLE_H - BUILD_H) / 2;
    setView(padding, padding + BUILD_W, padding, padding + BUILD_H);

    buildFrame = new Frame(this, mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost);
    new WaterSquare(this, mLeftMost, mRightMost, mDownMost, mCurDownMost - 1.0f);

    memset(buttons, 0, sizeof buttons);
    float curH = mCurUpMost;

    curH -= 0.9f;
    buttons[BUTTON_SMALL_WOOD_BLOCK].first = new SmallWoodBlock(this, mCurLeftMost + 1.2f, curH);
    buttons[BUTTON_SMALL_WOOD_BLOCK].second = new NewObjectCallback<SmallWoodBlock>(mMouseHandler);
    buttons[BUTTON_SMALL_WOOD_BLOCK].first->getReferee()->SetType(b2_staticBody);
    curH -= 0.9f;

    curH -= 1.9f;
    buttons[BUTTON_LARGE_WOOD_BLOCK].first = new LargeWoodBlock(this, mCurLeftMost + 2.2f, curH);
    buttons[BUTTON_LARGE_WOOD_BLOCK].second = new NewObjectCallback<LargeWoodBlock>(mMouseHandler);
    buttons[BUTTON_LARGE_WOOD_BLOCK].first->getReferee()->SetType(b2_staticBody);
    curH -= 1.9f;

    curH -= 1.3f;
    buttons[BUTTON_STEEL_STICK].first = new SteelStick(this, mCurLeftMost + 0.4f, curH + 0.7f, mCurLeftMost + 1.8f, curH - 0.7f);
    buttons[BUTTON_STEEL_STICK].second = new NewObjectCallback<SteelStick>(mMouseHandler);
    buttons[BUTTON_STEEL_STICK].first->getReferee()->SetType(b2_staticBody);
    curH -= 1.3f;

    curH -= 1.1f;
    buttons[BUTTON_DELETE].first = new Button<IMAGE_RED_CROSS>(this, mCurLeftMost + 0.8f, mCurLeftMost + 1.6f, curH - 0.4f, curH + 0.4f);
    buttons[BUTTON_DELETE].second = new DeleteButtonCallback(mMouseHandler);
    curH -= 1.1f;

    for (int i = 0; i < BUTTON_NUM; i++)
        mMouseHandler->addButton(buttons[i].first, buttons[i].second);
}

MainWorld::~MainWorld()
{
    for (int i = 0; i < BUTTON_NUM; i++)
        if (buttons[i].second)
            delete buttons[i].second; // delete callbacks
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
