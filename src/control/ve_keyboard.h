//
// Created by boris on 11/19/2021.
//

#ifndef VENGINE3D_VE_KEYBOARD_H
#define VENGINE3D_VE_KEYBOARD_H

#include "ve_keyboardkey.h"
#include "ve_keystate.h"
#include "stdlibraries.h"

namespace VE{
    class Keyboard {
    public:
        Keyboard() {
            for (KeyState &key: keyState) {
                key = VE_KEY_FREE;
            }
            for (bool &r: repeatStatus) {
                r = false;
            }
        }

        void setKeyState(int key, KeyState state) {
            keyState[key] = state;
            if (keyState[key] == VE_KEY_PRESSED) {
                repeatStatus[key] = true;
            } else if (keyState[key] == VE_KEY_RELEASE) {
                repeatStatus[key] = false;
            }
        }

        bool isRelease(int key) {
            if (keyState[key] == VE_KEY_RELEASE) {
                keyState[key] = VE_KEY_FREE;
                return true;
            }
            return false;
        }

        bool isPressed(int key) {
            if (keyState[key] == VE_KEY_PRESSED) {
                keyState[key] = VE_KEY_FREE;
                return true;
            }
            return false;
        }

        bool isRepeat(int key) {
            if (keyState[key] == VE_KEY_REPEAT || repeatStatus[key]) {
                keyState[key] = VE_KEY_FREE;
                return true;
            }
            return false;
        }


    private:
        KeyState keyState[512];
        bool repeatStatus[512];
    };

    using KeyboardPtr = std::shared_ptr<Keyboard>;
}



#endif //VENGINE3D_VE_KEYBOARD_H
