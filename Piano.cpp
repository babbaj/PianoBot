//
// Created by babbaj on 5/2/2019.
//

#include "Piano.h"
#include "Keys.h"
#include "ShiftGuard.h"


#include <memory>
#include <vector>
#include <iostream>
#include <regex>
#include <cctype>

void playMultiNote(const Piano* piano, const Note& note) {
    assert(note.type == NoteType::MULTI);
}

static const std::vector<std::string> splitByRegex(const std::string &input, const std::regex &pattern) {
    std::sregex_iterator iter(input.begin(), input.end(), pattern); // std::regex_search is literally broken
    std::sregex_iterator iter_end;
    std::vector<std::string> args;

    std::for_each(iter, iter_end, [&](auto& match) {
        args.push_back(match.str());
    });

    return args;
}

bool isMulti(const std::string& regex_token) {
    static const std::regex multi_regex(R"RE(\[[^\[\]]+\])RE");
    return std::regex_match(regex_token, multi_regex);
}

const std::vector<Note> parseNoteFile(std::ifstream &stream) {
    static const std::regex regex(R"RE([^\[\]]+|(\[.+\]))RE");

    const std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    const auto vec = splitByRegex(str, regex);

    std::vector<Note> out;
    for (const auto& token : vec) {
        if (isMulti(str)) {
            out.push_back(Note::multiNote(token.begin() + 1, token.end() - 1));
        } else {
            for (char c : token) {
                if (c == ' ' || PAUSE_CHARS.find(c) != PAUSE_CHARS.end()) {
                    out.push_back(Note::silentNote(c));
                } else if (VALID_KEYS.find(c) != VALID_KEYS.end()) {
                    out.push_back(Note::singletonNote(c));
                } else {
                    // ignored
                }
            }
        }
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
    if (key >= 97 && key <= 122) {
        key = std::toupper(key); // TODO: put this somewhere else
    }
    SendMessage(piano.hWindowHandle, WM_KEYDOWN, key, 0x1);
    Sleep(NO_DELAY);
    SendMessage(piano.hWindowHandle, WM_KEYUP, key, 0x1);
}

using key_iterator = std::vector<char>::const_iterator;

void play(const Piano& piano, const Note& note, std::unique_ptr<ShiftGuard>& shift_ptr) {
    if (note.type == NoteType::SILENT) {
        Sleep(note.delay);
        return;
    }

    for (auto key : note.keys) {
        if (isBlackKey(key)) {
            if (!shift_ptr) shift_ptr = std::make_unique<ShiftGuard>(piano); // ensure we have a shift guard
        } else {
            shift_ptr.reset(); // destroy any existing shift guard
        }
        playNote(piano, key);
        Sleep(note.multi_key_delay);
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
            ::play(*this, note, optShift);
        }
    }

}

