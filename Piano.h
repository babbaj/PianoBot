#pragma once

#include <Windows.h>

#include "Note.h"

#include <fstream>


class Piano {
public:
    HWND hWindowHandle;

    explicit Piano(HWND windowHandle): hWindowHandle(windowHandle)
    {}

    void play(const std::string &file) noexcept(false); // throws std::ifstream::failure
};


