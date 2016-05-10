#include "window.h"
#include "world.h"

int main()
{
    Window window;
    TestWorldButtons world;
    window.setWorld(&world);
    window.run();
    return 0;
}

