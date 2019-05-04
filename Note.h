#pragma once

#include <vector>
#include <cassert>
#include <functional>
#include <set>
#include <type_traits>

#include <Windows.h>


constexpr auto NOTE_LENGTH = 150;

constexpr auto FAST_DELAY = 50; // {}
constexpr auto NO_DELAY = 3; // [] play together



class Note;

using note_player_t = auto(*) (Note&) -> void;

enum class NoteType {
    SINGLETON,
    MULTI,
    SILENT // used for delay
};

class Note {
public:
    const NoteType type;

    const std::vector<char> keys;

    union {
        // Delay between each of this note's keys.
        // For [] notes this should be NO_DELAY
        // For Single and Silent keys this will be the same value as delay (this is a union)
        const unsigned int multi_key_delay;

        // delay between this note and the next
        // only for silent notes
        const unsigned int delay;
    };


    static Note singletonNote(char key) {
        return Note{NoteType::SINGLETON, {key}, 0};
    }

    static Note silentNote(unsigned int length) {
        return Note{NoteType::SILENT, {}, length};
    }

    template<typename Iter>
    Note fastMultiNote(Iter begin, Iter end) { // {} groups // TODO: delete this
        assert(begin != end);
        return Note{NoteType::MULTI, {begin, end}, FAST_DELAY};
    }

    template<typename Iter>
    static Note multiNote(Iter begin, Iter end) { // []/() groups
        assert(begin != end);
        return Note{NoteType ::MULTI, {begin, end}, NO_DELAY};
    }

};

using song_t = std::vector<Note>; // TODO: song struct?





