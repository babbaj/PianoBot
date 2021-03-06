#pragma once

#include <exception>
#include <iostream>
#include "Piano.h"

constexpr auto SHIFT_LPARAM = 0x002C0001; // includes scancode


// RAII type
class ShiftGuard {
    const Piano &piano;

    static bool isHeld; // runtime borrow checking :^(

public:
    explicit ShiftGuard(const Piano &_piano): piano(_piano) {
        if (isHeld) {
            throw std::exception("Shift is already being held");
        }
        isHeld = true;
        SendMessage(piano.hWindowHandle, WM_KEYDOWN, VK_SHIFT, SHIFT_LPARAM);
        Sleep(NO_DELAY); // not sure if this fixes anything
    }

    ~ShiftGuard() {
        isHeld = false;
        SendMessage(piano.hWindowHandle, WM_KEYUP, VK_SHIFT, SHIFT_LPARAM);
        Sleep(NO_DELAY);
    }

    ShiftGuard(const ShiftGuard&) = delete; // can not be copied, there should one ever be 1 instance in existence
    ShiftGuard(ShiftGuard&&) = delete;

};

bool ShiftGuard::isHeld = false;