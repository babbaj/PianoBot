#pragma once

#include <vector>
#include <cassert>
#include <functional>
#include <set>
#include <type_traits>

#include <Windows.h>


constexpr auto NOTE_LENGTH = 200;

constexpr auto FAST_DELAY = 100; // {}
constexpr auto NO_DELAY = 1; // [] play together



class Note;

using note_player_t = auto(*) (Note&) -> void;

//void playMultiNote(Note& note);
//void playSingleNote(Note& note);
//void playSilentNote(Note& note);

enum class NoteType {
    SINGLETON,
    MULTI,
    SILENT // used for delay
};

class Note {
public:
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

    static Note silentNote(char key) {
        return Note{NoteType::SILENT, {}, NOTE_LENGTH};
    }

    template<typename Iter>
    Note fastMultiNote(Iter begin, Iter end) { // {} groups
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





