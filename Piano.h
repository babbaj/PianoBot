#pragma once

#include <Windows.h>

#include "Note.h"

#include "MidiFile.h"

#include <fstream>
#include <optional>
#include <variant>


class Piano {
public:
    HWND hWindowHandle;

    //std::optional<std::vector<Note>> loaded_song;
    std::variant<std::vector<Note>, smf::MidiFile> loaded_song;

    explicit Piano(HWND windowHandle): hWindowHandle(windowHandle)
    {}

    Piano(const Piano&) = delete;

    void loadText(const char* file) noexcept(false); // throws std::ifstream::failure
    void loadMidi(const char* file) noexcept(false);


    void play();
};


