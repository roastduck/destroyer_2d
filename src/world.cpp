#include <GLFW/glfw3.h>
#include "world.h"

World::World(int leftMost, int rightMost, int downMost, int upMost)
    : mLeftMost(leftMost), mRightMost(rightMost),
      mDownMost(downMost), mUpMost(upMost),
      mCurLeftMost(leftMost), mCurRightMost(rightMost),
      mCurDownMost(downMost), mCurUpMost(upMost)
{}

void World::setGLOrtho() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost, 1.0f, -1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void World::drawAll() const
{
    //TODO
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

#endif // COMPILE_TEST
