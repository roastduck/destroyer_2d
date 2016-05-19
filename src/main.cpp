#include "window.h"
#include "world.h"

int main()
{
    Window window;
    window.setWorld(new MainWorld() /*TestWorldFullView(1)*/);
    // Window will take charge of destruction of World
    window.run();
    return 0;
}

