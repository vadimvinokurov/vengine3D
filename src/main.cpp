#include <iostream>
#include "ve_window.h"

int main() {
    VE::WorldPtr world(new VE::World);

    VE::Window window;
    window.setWorld(world);
    window.run();
    return 0;
}

