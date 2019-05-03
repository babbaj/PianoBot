#pragma once

#include <vector>
#include <cassert>
#include <functional>

#include <Windows.h>


constexpr auto FAST_DELAY = 100; // {}
constexpr auto NO_DELAY = 1; // [] play together

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
    const NoteType type; // probably not gonna need this

    const std::vector<char> keys;
    // Delay between each of this note's keys.
    // For []/() notes this should be 0
    // For {} notes this should be set to FAST_DELAY (100)
    // For Single and 0 note keys this can be anything
    const unsigned int multi_key_delay;

    //const note_player_t notePlayer;
    Note(const Note& other) = default;

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





