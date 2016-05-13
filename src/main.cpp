#include "window.h"
#include "world.h"

int main()
{
    Window window;
    MainWorld world;
    //TestWorldSimplePhysics world;
    window.setWorld(&world);
    window.run();
    return 0;
}

