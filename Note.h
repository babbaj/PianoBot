#pragma once

#include <vector>
#include <cassert>
#include <functional>
#include <set>
#include <type_traits>

#include <Windows.h>


constexpr auto NOTE_LENGTH = 150;

constexpr auto FAST_DELAY = 100; // {}
constexpr auto NO_DELAY = 3; // [] play together


enum class NoteType { // TODO: delete this?
    SINGLE,
    MULTI,
    SILENT
};

class Note {
public:
    const NoteType type;

    const std::vector<char> keys;

    // delay between this note and the next
    const unsigned int delay;


    static Note singleNote(char key) {
        return Note{NoteType::SINGLE, {key}, NOTE_LENGTH};
    }

    static Note silentNote(unsigned int delay_len) {
        return Note{NoteType::SILENT, {}, delay_len};
    }

    template<typename Iter>
    static Note multiNote(Iter begin, Iter end) { // [] groups
        assert(begin != end);
        return Note{NoteType ::MULTI, {begin, end}, NOTE_LENGTH};
    }

};




