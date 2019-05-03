//
// Created by babbaj on 5/2/2019.
//

#include "Piano.h"


#include <memory>
#include <vector>
#include <iostream>


// TODO: parse note files properly
const std::vector<Note> parseNoteFile(std::ifstream &stream) {
    const std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    std::vector<Note> out;
    for (char c : str) {
        out.push_back(Note::singletonNote(c));
    }
    return out;
}

void Piano::play(const std::string &file) noexcept(false) {
    std::ifstream stream(file.c_str());
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    auto notes = parseNoteFile(stream);
    for (auto &note : notes) {
        for (char key : note.keys) {
            SendMessage(hWindowHandle, WM_KEYDOWN, key, 0x1);
            Sleep(5);
            SendMessage(hWindowHandle, WM_KEYUP, key, 0x1);

            Sleep(200);
        }
    }

}