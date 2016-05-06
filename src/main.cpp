#include "window.h"
#include "world.h"

int main()
{
    Window window;
    TestWorldSimplePhysics world;
    window.setWorld(&world);
    window.run();
    return 0;
}

