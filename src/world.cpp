#include "world.h"
#include "render.h"
#include "matter.h"
#include "mousecallback.h"

void World::MyDestructionListener::SayGoodbye(b2Joint* joint)
{
    for (auto s : subscribers) s->SayGoodbye(joint);
}

void World::MyDestructionListener::SayGoodbye(b2Fixture *fixture)
{
    for (auto s : subscribers) s->SayGoodbye(fixture);
}

void World::MyDestructionListener::SayGoodbye(b2ParticleGroup *group)
{
    for (auto s : subscribers) s->SayGoodbye(group);

    b2ParticleSystem *system = group->GetParticleSystem();
    assert(system->GetParticleGroupCount() > 0);
    if (system->GetParticleGroupCount() == 1)
        ParticleSystem::setDied((ParticleSystem*)(system->GetParticleGroupList()->GetUserData()));
}

void World::MyDestructionListener::subscribe(b2DestructionListener *p)
{
    subscribers.push_back(p);
}

void World::MyDestructionListener::unsubscribe(b2DestructionListener *p)
{
    subscribers.erase(std::find(subscribers.begin(), subscribers.end(), p));
}

World::MyDestructionListener World::myDestructionListener;

void World::MyContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    b2Body *b1 = contact->GetFixtureA()->GetBody(), *b2 = contact->GetFixtureB()->GetBody();
    Rigid *r1 = b1->GetType() == b2_dynamicBody ? (Rigid*)(b1->GetUserData()) : NULL;
    Rigid *r2 = b2->GetType() == b2_dynamicBody ? (Rigid*)(b2->GetUserData()) : NULL;
    for (int i = 0; i < impulse->count; i++)
    {
        if (r1 && impulse->normalImpulses[i] > r1->getStrength())
            destroying.push_back(r1);
        if (r2 && impulse->normalImpulses[i] > r2->getStrength())
            destroying.push_back(r2);
    }
}

bool World::GlobalTestPoint::ReportFixture(b2Fixture *_f)
{
    if (_f->TestPoint(p))
    {
        fixture = _f; // answer
        return false; // stop
    } else
        return true; // continue
}

World::World(float leftMost, float rightMost, float downMost, float upMost) noexcept
    : mWindow(NULL), mMouseHandler(new MouseHandler(this)),
      mLeftMost(leftMost), mRightMost(rightMost),
      mDownMost(downMost), mUpMost(upMost),
      mCurLeftMost(leftMost), mCurRightMost(rightMost),
      mCurDownMost(downMost), mCurUpMost(upMost),
      physics(new b2World(b2Vec2(0.0f, -GRAVITY)))
{
    physics->SetDestructionListener(&myDestructionListener);
    physics->SetContactListener(&myContactListener);

    frameBody = (new Frame(this, leftMost, rightMost, downMost, upMost))->getReferee();
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

void World::examContact()
{
    sort(myContactListener.destroying.begin(), myContactListener.destroying.end());
    for (auto i = myContactListener.destroying.begin(); i != myContactListener.destroying.end(); i++)
        if (i == myContactListener.destroying.begin() || *i != *(i-1))
            (*i)->damage();
    myContactListener.destroying.clear();
}

void World::drawAll() const noexcept
{
    for (const b2Body *b = physics->GetBodyList(); b; b = b->GetNext())
        Render::getInstance().drawRigid(b, getScale());

    for (const b2ParticleSystem *s = physics->GetParticleSystemList(); s; s = s->GetNext())
        Render::getInstance().drawParticleSystem(s, getScale());
}

void World::step()
{
    mMouseHandler->process();
    examContact();
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

float World::getScale() const
{
    int px, py;
    mWindow->getWidthHeight(&px, &py);
    return ((mCurRightMost - mCurLeftMost) / px + (mCurUpMost - mCurDownMost) / py) / 2;
}

class LaunchCallback : public MouseCallback
{
public:
    LaunchCallback(MouseHandler *_handler) : MouseCallback(_handler) {}
    void leftClick(float, float) { ((MainWorld*)(mMouseHandler->getWorld()))->notifyLaunch(); }
};

class CancelCallback : public MouseCallback
{
public:
    CancelCallback(MouseHandler *_handler) : MouseCallback(_handler) {}
    void leftClick(float, float) { ((MainWorld*)(mMouseHandler->getWorld()))->cancelBattle(); }
};

/// This is called during construction
void MainWorld::makeBuildingButtons()
{
    // Mousehandler will take charge of destructions of button rigids and callbacks
    std::pair<Rigid*, MouseCallback*> buttons[BUTTON_NUM];
    memset(buttons, 0, sizeof buttons);
    float curH = mCurUpMost;
    float mx = (mCurLeftMost+mCurRightMost)/2;

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

    curH -= 0.9f;
    buttons[BUTTON_SMALL_STEEL_BALL].first = new SmallSteelBall(this, mCurLeftMost + 1.2f, curH);
    buttons[BUTTON_SMALL_STEEL_BALL].second = new NewObjectCallback<SmallSteelBall>(mMouseHandler);
    buttons[BUTTON_SMALL_STEEL_BALL].first->getReferee()->SetType(b2_staticBody);
    curH -= 0.9f;

    curH -= 1.3f;
    buttons[BUTTON_STEEL_STICK].first = new SteelStick(this, mCurLeftMost + 0.4f, curH + 0.7f, mCurLeftMost + 1.8f, curH - 0.7f);
    buttons[BUTTON_STEEL_STICK].second = new NewObjectCallback<SteelStick>(mMouseHandler);
    buttons[BUTTON_STEEL_STICK].first->getReferee()->SetType(b2_staticBody);
    curH -= 1.3f;

    curH -= 3.0f;
    buttons[BUTTON_DELETE].first = new Button<IMAGE_RED_CROSS>(this, mCurLeftMost + 0.8f, mCurLeftMost + 1.6f, curH - 0.4f, curH + 0.4f);
    buttons[BUTTON_DELETE].second = new DeleteButtonCallback(mMouseHandler);
    curH -= 1.1f;

    buttons[BUTTON_LAUNCH].first = new Button<IMAGE_LAUNCH>(this, mx - 2.0f, mx + 2.0f, mCurUpMost - 2.0f, mCurUpMost - 1.0f);
    buttons[BUTTON_LAUNCH].second = new LaunchCallback(mMouseHandler);

    for (int i = 0; i < BUTTON_NUM; i++)
        mMouseHandler->addButton(buttons[i].first, buttons[i].second);
}

MainWorld::MainWorld()
    : World(0, BATTLE_W, 0, BATTLE_H), status(STATUS_BUILDING)
{
    float padding = (BATTLE_H - BUILD_H) / 2;
    setView(padding, padding + BUILD_W, padding, padding + BUILD_H);

    buildFrame = new Frame(this, mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost);
    new WaterSquare(this, mLeftMost, mRightMost, mDownMost, mCurDownMost - 1.0f);

    makeBuildingButtons();

    mMouseHandler->setFreeCallback(new DraggingCallback(mMouseHandler));
}

void MainWorld::makeBattleButtons()
{
    cancelButton = new Button<IMAGE_RED_CROSS>(this, -0.4f, 0.4f, -0.4f, 0.4f);
    cancelButton->getReferee()->SetActive(false);
    mMouseHandler->addButton(cancelButton, new CancelCallback(mMouseHandler));
}

void MainWorld::launch()
{
    status = STATUS_BATTLE;
    mMouseHandler->cleanButtons();
    mMouseHandler->setFreeCallback(NULL);
    makeBattleButtons();

    delete buildFrame;
    buildFrame = NULL;
}

void MainWorld::focus()
{
    float l(INFINITY), r(-INFINITY), d(INFINITY), u(-INFINITY);
    for (b2Body *b = physics->GetBodyList(); b; b = b->GetNext())
        if (((Matter*)(b->GetUserData()))->getIsUserCreated())
        {
            b2Vec2 pos(b->GetPosition());
            l = std::min(l, pos.x - 9.0f);
            r = std::max(r, pos.x + 27.0f);
            d = std::min(d, pos.y - 9.0f);
            u = std::max(u, pos.y + 9.0f);
        }
    if (l == INFINITY)
        l = mLeftMost, r = mRightMost, d = mDownMost, u = mUpMost;

    if ((r - l) < (u - d) * 1.33f)
    {
        float m = (l + r) / 2;
        l = m - (u - d) * 1.33f / 2;
        r = m + (u - d) * 1.33f / 2;
    } else
    {
        float m = (d + u) / 2;
        d = m - (r - l) / 1.33f / 2;
        u = m + (r - l) / 1.33f / 2;
    }

    l = std::max(l, mLeftMost);
    r = std::min(r, mRightMost);
    d = std::max(d, mDownMost);
    u = std::min(u, mUpMost);

    assert(l <= r && d <= u);
    if ((r - l) > (u - d) * 1.33f)
    {
        float m = (l + r) / 2;
        l = m - (u - d) * 1.33f / 2;
        r = m + (u - d) * 1.33f / 2;
    } else
    {
        float m = (d + u) / 2;
        d = m - (r - l) / 1.33f / 2;
        u = m + (r - l) / 1.33f / 2;
    }

    float dl = std::max(std::min(l - mCurLeftMost, FOCUS_SPEED), -FOCUS_SPEED);
    float dr = std::max(std::min(r - mCurRightMost, FOCUS_SPEED), -FOCUS_SPEED);
    float dd = std::max(std::min(d - mCurDownMost, FOCUS_SPEED), -FOCUS_SPEED);
    float du = std::max(std::min(u - mCurUpMost, FOCUS_SPEED), -FOCUS_SPEED);
    setView(mCurLeftMost+dl, mCurRightMost+dr, mCurDownMost+dd, mCurUpMost+du);

    cancelButton->getReferee()->SetTransform(
        b2Vec2(mCurLeftMost + 0.8f, mCurUpMost - 0.8f),
        cancelButton->getReferee()->GetAngle()
    );
}

void MainWorld::step()
{
    switch (status)
    {
    case STATUS_BUILDING:
        break;
    case STATUS_BATTLE:
        focus();
        break;
    case STATUS_NOTIFY_LAUNCH:
        launch();
        break;
    case STATUS_CANCEL_BATTLE:
        mWindow->setWorld(new MainWorld());
        return;
    }
    World::step();
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
    new LargeWoodBlock(this, -5.0f, 5.0f);
    new SmallSteelBall(this, -4.0f, 8.0f);
    new WaterSquare(this, -10.0f, 10.0f, -10.0f, 0.0f);
}

#endif // COMPILE_TEST
