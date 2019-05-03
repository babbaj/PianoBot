#pragma once

#include <exception>
#include "Piano.h"

constexpr auto SHIFT_LPARAM = 0x002C0001; // includes scancode


// RAII type
// TODO: rule of 5
struct ShiftGuard {
    const Piano &piano;

    static bool isHeld;

    explicit ShiftGuard(const Piano &_piano): piano(_piano) {
        if (isHeld) {
            throw std::exception("Shift is already being held");
        }
        isHeld = true;
        SendMessage(piano.hWindowHandle, WM_KEYDOWN, VK_SHIFT, SHIFT_LPARAM);
    }

    ~ShiftGuard() {
        isHeld = false;
        SendMessage(piano.hWindowHandle, WM_KEYUP, VK_SHIFT, SHIFT_LPARAM);
    }

};

bool ShiftGuard::isHeld = false;