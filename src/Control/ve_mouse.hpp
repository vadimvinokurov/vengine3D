//
// Created by boris on 11/19/2021.
//

#ifndef VENGINE3D_VE_MOUSE_HPP
#define VENGINE3D_VE_MOUSE_HPP

#include "ve_mousekey.hpp"
#include "stdlibraries.h"

namespace VE {

    class Mouse {
    public:
        Mouse() : position_(0, 0) {
            for (KeyState &key: keyState) {
                key = VE_KEY_FREE;
            }
            for (bool &r: repeatStatus) {
                r = false;
            }
        }

        const VE::Vector &position() const {
            return position_;
        }

        VE::Vector deltaPosition() const {
            return position_ - lockPosition_;
        }

        void setPosition(const VE::Vector &position) {
            position_ = position;
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

        bool isLock() const {
            return lock;
        }

        void lockMouse() {
            lock = true;
        }

        void unlockMouse() {
            lock = false;
        }

        void setLockPosition() {
            lockPosition_ = position_;
        }

        const Vector &lockPosition() {
            return lockPosition_;
        }

    private:
        VE::Vector position_;
        VE::Vector lockPosition_;
        KeyState keyState[8];
        bool repeatStatus[8];
        bool lock = false;
    };

    using MousePtr = std::shared_ptr<Mouse>;
}


#endif //VENGINE3D_VE_MOUSE_HPP
