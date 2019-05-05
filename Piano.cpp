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


std::vector<std::string> groups(const std::string& str, const std::regex& regex) { // this is pretty bad
    std::sregex_token_iterator non_matching_iter(str.begin(), str.end(), regex, -1);
    std::sregex_token_iterator non_matching_end;

    std::sregex_iterator matching_iter(str.begin(), str.end(), regex);
    std::sregex_iterator matching_end;

    std::vector<std::string> out;

    const auto run_matching = [&]{
        if (matching_iter != matching_end) {
            out.push_back(matching_iter->str());
            matching_iter++;
        }
    };
    const auto run_not_matching = [&]{
        if (non_matching_iter != non_matching_end) {
            if (!non_matching_iter->str().empty()) {
                out.push_back(*non_matching_iter);
            }
            non_matching_iter++;
        }
    };


    const auto matchingFirst = [&]{
        while(matching_iter != matching_end || non_matching_iter != non_matching_end) {
            run_matching();
            run_not_matching();
        }
    };
    const auto nonMatchingFirst = [&]{
        while(matching_iter != matching_end || non_matching_iter != non_matching_end) {
            run_not_matching();
            run_matching();
        }
    };


    if (matching_iter != matching_end && matching_iter->position(0) == 0) {
        non_matching_iter++; // skip first empty string

        // add matching first
        matchingFirst();

    } else {
        // add non matching first
        nonMatchingFirst();
    }


    return out;
}

bool matches(const std::string& regex_token, const std::regex& regex) { // TODO: i shouldnt need this
    return std::regex_match(regex_token, regex);
}

const std::vector<Note> parseNoteFile(std::ifstream &stream) {
    static const std::regex multi_regex(R"RE(\[[^\[\]]+\])RE");

    const std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    const auto vec = groups(str, multi_regex);

    std::vector<Note> out;
    for (const auto& token : vec) {
        if (matches(token, multi_regex)) {
            out.push_back(Note::multiNote(token.begin() + 1, token.end() - 1));
        } else {
            for (char c : token) {
                if (c == ' ' || PAUSE_CHARS.find(c) != PAUSE_CHARS.end()) {
                    out.push_back(Note::silentNote(c == ' ' ? NOTE_LENGTH : NOTE_LENGTH * 4)); // TODO: find good pause time
                } else if (VALID_KEYS.find(c) != VALID_KEYS.end()) {
                    out.push_back(Note::singleNote(c));
                } else {
                    // ignored
                }
            }
        }
    }

    return out;
}


void Piano::load(const char* file) noexcept(false) {
    std::ifstream stream(file);
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    auto notes = parseNoteFile(stream);
    stream.close();
    this->loaded_song = std::move(notes);
}

void playKey(const Piano &piano, char key) {
    SendMessage(piano.hWindowHandle, WM_KEYDOWN, key, 0x1);
    Sleep(NO_DELAY);
    SendMessage(piano.hWindowHandle, WM_KEYUP, key, 0x1);
}

void play(const Piano& piano, const Note& note, std::unique_ptr<ShiftGuard>& shift_ptr) {
    if (note.type == NoteType::SILENT) {
        Sleep(note.delay);
        return;
    }

    for (const auto key : note.keys) {
        if (isBlackKey(key)) {
            if (!shift_ptr) shift_ptr = std::make_unique<ShiftGuard>(piano); // ensure we have a shift guard
            playKey(piano, BLACK_KEYS.at(key));
        } else {
            shift_ptr.reset(); // destroy any existing shift guard
            // uppercase letters must be sent
            const char upper = (key >= 'a' && key <= 'z') ? key - 32 : key;
            playKey(piano, upper);
        }
        Sleep(note.multi_key_delay);
    }
    Sleep(NOTE_LENGTH);
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

