//
// Created by boris on 11/19/2021.
//

#ifndef VENGINE3D_VE_MOUSE_HPP
#define VENGINE3D_VE_MOUSE_HPP

#include "ve_mousekey.hpp"
#include "stdlibraries.h"

namespace VE{

    class Mouse {
    public:
        Mouse() : mousePosition_(0, 0) {
            for (KeyState &key: keyState) {
                key = VE_KEY_FREE;
            }
        }

        const VE::Vector &mousePosition() const {
            return mousePosition_;
        }

        void setMousePosition(const VE::Vector &mousePosition) {
            mousePosition_ = mousePosition;
        }

        void setKeyState(int key, KeyState state) {
            keyState[key] = state;
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
            if (keyState[key] == VE_KEY_REPEAT) {
                keyState[key] = VE_KEY_FREE;
                return true;
            }
            return false;
        }

    private:
        VE::Vector mousePosition_;
        KeyState keyState[3];
    };
    using MousePtr = std::shared_ptr<Mouse>;
}


#endif //VENGINE3D_VE_MOUSE_HPP
