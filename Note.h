#pragma once

#include <vector>
#include <cassert>
#include <functional>
#include <set>

#include <Windows.h>


constexpr auto FAST_DELAY = 100; // {}
constexpr auto NO_DELAY = 1; // [] play together

const std::set<char> VALID_KEYS = {
    '1', '!', '2', '@', '3', '4', '$', '5', '%', '6', '^', '7', '8', '*', '9', '(', '0',
    'q', 'Q', 'w', 'W', 'e', 'E', 'r', 't', 'T', 'y', 'Y', 'u', 'i', 'I', 'o', 'O', 'p', 'P', 'a', 's', 'S', 'd', 'D', 'f', 'g', 'G', 'h', 'H', 'j', 'J', 'k', 'l', 'L', 'z', 'Z', 'x', 'c', 'C', 'v', 'V', 'b', 'B', 'n', 'm'
};

const std::set<char> GROUP_CHARS = {
    '[', ']', '(', ')',
    '{', '}'
};





struct Note;

using note_player_t = auto(*) (Note&) -> void;

void playMultiNote(Note& note);
void playSingleNote(Note& note);
void playSilentNote(Note& note);

enum class NoteType {
    SINGLETON,
    FAST_MULTI,
    MULTI,
    SILENT // used for delay
};

struct Note {
    const NoteType type;

    const std::vector<char> keys;
    // Delay between each of this note's keys.
    // For []/() notes this should be 0
    // For {} notes this should be set to FAST_DELAY (100)
    // For Single and 0 note keys this can be anything
    const unsigned int multi_key_delay;


    static Note singletonNote(char key) {
        return Note{NoteType::SINGLETON, {key}, 0};
    }

    static Note FastMultiNote(std::initializer_list<char> keys) { // {} groups
        assert(keys.size() > 0);
        return Note{NoteType::FAST_MULTI, {keys}, FAST_DELAY};
    }

    static Note MultiNote(std::initializer_list<char> keys) { // []/() groups
        assert(keys.size() > 0);
        return Note{NoteType ::MULTI, {keys}, NO_DELAY};
    }

};

using song_t = std::vector<Note>; // TODO: song struct?





