/**
 * Forwards mouse actions to Matter
 */

#ifndef MOUSEHANDLER_H_
#define MOUSEHANDLER_H_

#include <list>
#include <utility>

class World;
class Rigid;

class OnClickCallback
{
public:
    virtual ~OnClickCallback() {}
    virtual void what() {}
};

class MouseHandler
{
public:
    enum MouseStatus
    {
        MOUSE_FREE = 0,
        MOUSE_PUTTING = 1, // putting a size-determined object
        MOUSE_DRAWING = 2 // drawing a line
    };

    MouseHandler(World *_world) : mWorld(_world), status(MOUSE_FREE) {}

    void addButton(Rigid *_rigid, OnClickCallback *_callback);

    void process();

    void updateMouse();

private:
    void processFree();

    World *mWorld;

    float worldX, worldY; // cursor in world coordinates

    MouseStatus status;

    std::list< std::pair<Rigid*, OnClickCallback*> > buttons;
};

#endif // MOUSEHANDLER_H_

