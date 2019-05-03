//
// Created by babbaj on 5/2/2019.
//

#include "Piano.h"
#include "Keys.h"
#include "ShiftGuard.h"


#include <memory>
#include <vector>
#include <iostream>

void playMultiNote(const Piano* piano, const Note& note) {
    assert(note.type == NoteType::MULTI || note.type == NoteType::FAST_MULTI);
}

void playSingleNote(const Piano* piano, const Note& note) {
    assert(note.type == NoteType::SINGLETON);
    SendMessage(piano->hWindowHandle, WM_KEYDOWN, note.keys[0], 0x1);
    Sleep(5);
    SendMessage(piano->hWindowHandle, WM_KEYUP, note.keys[0], 0x1);
}



// TODO: parse note files properly
const std::vector<Note> parseNoteFile(std::ifstream &stream) {
    const std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    std::vector<Note> out;
    for (char c : str) {
        out.push_back(Note::singletonNote(c));
    }
    return out;
}


void Piano::load(const std::string &file) noexcept(false) {
    std::ifstream stream(file.c_str());
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    auto notes = parseNoteFile(stream);
    this->loaded_song = std::move(notes);
}

void playNote(const Piano& piano, char key) {
    SendMessage(piano.hWindowHandle, WM_KEYDOWN, key, 0x1);
    Sleep(5);
    SendMessage(piano.hWindowHandle, WM_KEYUP, key, 0x1);
    Sleep(200);
}

using key_iterator = std::vector<char>::const_iterator;

void play(const Piano& piano, const key_iterator iter, const key_iterator end, std::unique_ptr<ShiftGuard>& shift_ptr) {
    if (iter == end) return;

    if (const char key = *iter; isBlackKey(key)) {
        if (!shift_ptr) shift_ptr = std::make_unique<ShiftGuard>(piano); // ensure we have a shift guard
        playNote(piano, key);
        play(piano, iter+1, end, shift_ptr);
    } else {
        shift_ptr.reset(); // destroy any existing shift guard
        playNote(piano, key);
        play(piano, iter+1, end, shift_ptr);
    }
}


void Piano::play() {
    if (!this->loaded_song) {
        std::cerr << "No loaded song\n";
        return;
    }

    {
        std::unique_ptr<ShiftGuard> optShift;
        for (auto &note : *this->loaded_song) {
            ::play(*this, note.keys.cbegin(), note.keys.cend(), optShift);
        }
    }

    /*for (auto &note : *this->loaded_song) {
        //for (char key : note.keys) {
        //  SendMessage(hWindowHandle, WM_KEYDOWN, key, 0x1);
        //  Sleep(5);
        //  SendMessage(hWindowHandle, WM_KEYUP, key, 0x1);
        //
        //    Sleep(200);
        //}
        switch(note.type) {
            case NoteType::SINGLETON: {
                playSingleNote(this, note);

            } break;
            case NoteType::FAST_MULTI: {


            } break;
            case NoteType::MULTI: {


            } break;
            case NoteType::SILENT: {
                Sleep(500); // TODO: figure out good sleep time
            } break;

            default: throw std::exception("unknown enum");
        }


    }*/
}

