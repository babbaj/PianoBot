#pragma once

#include <Windows.h>

#include "Note.h"

#include <fstream>
#include <optional>


class Piano {
public:
    HWND hWindowHandle;

    std::optional<std::vector<Note>> loaded_song;

    explicit Piano(HWND windowHandle): hWindowHandle(windowHandle)
    {}

    Piano(const Piano&) = delete;

    void load(const char* file) noexcept(false); // throws std::ifstream::failure


    void play();
};


