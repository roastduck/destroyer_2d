#include <GLFW/glfw3.h>
#include "world.h"
#include "matter.h"

World::World(float leftMost, float rightMost, float downMost, float upMost)
    : mLeftMost(leftMost), mRightMost(rightMost),
      mDownMost(downMost), mUpMost(upMost),
      mCurLeftMost(leftMost), mCurRightMost(rightMost),
      mCurDownMost(downMost), mCurUpMost(upMost),
      physics(new b2World(b2Vec2(0.0f, -GRAVITY)))
{
    new Frame(this, leftMost, rightMost, downMost, upMost);
    // will keep track of it from LiquidFun
}

/**
 * delete Matter objects and then delete physics
 */
World::~World()
{
    for (b2Body *b = physics->GetBodyList(); b;)
    {
        b2Body *_b = b->GetNext();
        delete (Matter*)b->GetUserData(); // this will delete b2Body too
        b = _b;
    }

    // Destroy particle matter objects
    // TODO

    delete physics;
}

void World::setGLOrtho() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost, 1.0f, -1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void World::drawAll() const
{
    for (const b2Body *b = physics->GetBodyList(); b; b = b->GetNext())
    {
        const Matter *m = (const Matter*)b->GetUserData();
        for (const b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext())
        {
            const b2Shape *shape = f->GetShape();
            switch (shape->GetType())
            {
            case b2Shape::e_polygon:
                glBegin(GL_POLYGON);
                for (int i = 0; i < ((b2PolygonShape*)shape)->GetVertexCount(); i++)
                {
                    b2Vec2 pos(b->GetWorldPoint(((b2PolygonShape*)shape)->GetVertex(i)));
                    glColor4f(m->getColorR(), m->getColorG(), m->getColorB(), m->getColorA());
                    glVertex3f(pos.x, pos.y, 0.0f);
                }
                glEnd();
                break;

            default:
                assert(false);
            }
        }
    }

    // Draw particles
    //TODO
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
}

#ifdef COMPILE_TEST

TestWorldDisplayTriangle::TestWorldDisplayTriangle()
    : World(-10, 10, -10, 10)
{}

void TestWorldDisplayTriangle::drawAll() const
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
}

#endif // COMPILE_TEST
