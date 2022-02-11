//
// Created by boris on 11/19/2021.
//

#ifndef VENGINE3D_VE_MOUSE_H
#define VENGINE3D_VE_MOUSE_H

#include "ve_mousekey.h"

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

        const VE::Vector3 &position() const {
            return position_;
        }

        VE::Vector3 deltaPosition() const {
            return position_ - lockPosition_;
        }

        void setPosition(const VE::Vector3 &position) {
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
            lockPosition_ = position_;
            lock = true;
        }

        void unlockMouse() {
            lock = false;
        }

        const Vector3 &lockPosition() {
            return lockPosition_;
        }

    private:
        VE::Vector3 position_;
        VE::Vector3 lockPosition_;
        KeyState keyState[8];
        bool repeatStatus[8];
        bool lock = false;
    };

    using MousePtr = std::shared_ptr<Mouse>;
}


#endif //VENGINE3D_VE_MOUSE_H
