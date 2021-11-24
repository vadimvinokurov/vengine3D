#include <iostream>
#include "ve_window.h"
#include "Control/ve_keyboard.h"
#include "Control/ve_mouse.h"

int main() {
    VE::MousePtr mouse = std::make_shared<VE::Mouse>();
    VE::KeyboardPtr keyboard = std::make_shared<VE::Keyboard>();

    VE::WorldPtr world = std::make_shared<VE::World>();
    world->setHid(keyboard, mouse);

    VE::Window window;
    window.setWorld(world);
    window.setHid(keyboard, mouse);
    window.run();
    return 0;
}

