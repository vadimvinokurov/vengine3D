#include <iostream>
#include "Window.h"

int main() {

    VE::Window window;
    VE::WorldPtr world(new VE::World);
    window.setWorld(world);
    window.run();
    return 0;
}
