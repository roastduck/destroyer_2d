#include "window.h"
#include "world.h"

int main()
{
    Window window;
    TestWorldDisplayTriangle world;
    window.setWorld(&world);
    window.run();
    return 0;
}

