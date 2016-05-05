/**
 * All object in a scenario
 * Interact with OpenGL and LiquidFun
 */

#ifndef WORLD_H_
#define WORLD_H_

class World
{
public:
    /**
     * Initialize with the border of whole world
     */
    World(int leftMost, int rightMost, int downMost, int upMost);

    /**
     * Set OpenGL with orthographic projection
     * To disply corresponding area
     */
    void setGLOrtho() const;

    /**
     * Draw everything into OpenGL
     */
    virtual void drawAll() const;
    
private:
    // borders of the whole world and the displayed part in pixels
    int mLeftMost, mRightMost, mDownMost, mUpMost;
    int mCurLeftMost, mCurRightMost, mCurDownMost, mCurUpMost;
};

#ifdef COMPILE_TEST

/**
 * display a triangle
 */
class TestWorldDisplayTriangle : public World
{
public:
    TestWorldDisplayTriangle();
    void drawAll() const;
};

#endif // COMPILE_TEST

#endif // WORLD_H_

