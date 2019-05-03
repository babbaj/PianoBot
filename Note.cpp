#include "Note.h"

#include <unordered_map>

// map uppercase char to char that is used to create it
const std::unordered_map<char, char> BLACK_KEYS = {
    {'!', '1'},
    {'@', '2'},
    {'$', '4'},
    {'%', '5'},
    {'^', '6'},
    {'*', '8'},
    {'(', '9'},

    {'Q', 'Q'}, // TODO: simplify
    {'W', 'W'},
    {'E', 'E'},
    {'T', 'T'},
    {'Y', 'Y'},
    {'I', 'I'},
    {'O', 'O'},
    {'P', 'P'},
    {'S', 'S'},
    {'D', 'D'},
    {'G', 'G'},
    {'H', 'H'},
    {'J', 'J'},
    {'L', 'L'},
    {'Z', 'Z'},
    {'C', 'C'},
    {'V', 'V'},
    {'B', 'B'}
};

bool isBlackKey(const char key) {
    return BLACK_KEYS.find(key) != BLACK_KEYS.end();
}

void playNote(char key) {

}


void playMultiNote(Note& note) {
    assert(note.type == NoteType::MULTI || note.type == NoteType::FAST_MULTI);
}

void playSingleNote(Note& note) {
    assert(note.type == NoteType::SINGLETON);


}

void playSilentNote(Note& note) {
    assert(note.type == NoteType::SILENT);
    Sleep(500); // TODO: figure out good sleep time
}