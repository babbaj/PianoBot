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
#include <algorithm>



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
            out.push_back(Note::multiNote(token.begin() + 1, token.end() - 1)); // TODO: filter invalid keys
        } else {
            for (char c : token) {
                if (c == ' ' || PAUSE_CHARS.find(c) != PAUSE_CHARS.end()) {
                    out.push_back(Note::silentNote(c == ' ' ? NOTE_LENGTH : NOTE_LENGTH * 4));
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




void sendKey(const Piano &piano, char key) {
    SendMessage(piano.hWindowHandle, WM_KEYDOWN, key, 0x1);
    Sleep(NO_DELAY);
    SendMessage(piano.hWindowHandle, WM_KEYUP, key, 0x1);
}

void playKey(const Piano &piano, char key, std::unique_ptr<ShiftGuard> &shift_ptr) {
    if (isBlackKey(key)) {
        if (!shift_ptr) shift_ptr = std::make_unique<ShiftGuard>(piano); // ensure we have a shift guard
        sendKey(piano, BLACK_KEYS.at(key));
    } else {
        shift_ptr.reset(); // destroy any existing shift guard
        // uppercase letters must be sent
        const char upper = (key >= 'a' && key <= 'z') ? key - 32 : key;
        sendKey(piano, upper);
    }
}

void playNote(const Piano& piano, const Note &note, std::unique_ptr<ShiftGuard> &shift_ptr) {
    for (const auto key : note.keys) {
        playKey(piano, key, shift_ptr);
        if (note.type != NoteType::MULTI) break; // dont unnecessarily sleep

        Sleep(key == ' ' ? FAST_DELAY : NO_DELAY); // a space in a multinote is a very short delay
    }
}



void Piano::loadText(const char* file) noexcept(false) {
    std::ifstream stream(file);
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    auto notes = parseNoteFile(stream);
    stream.close();
    this->loaded_song = std::move(notes);
}

void removePercussionNotes(smf::MidiFile& midi) {
    for (int i = 0; i < midi.getTrackCount(); i++) {
        for (int j = 0; j < midi[i].getEventCount(); j++) {
            if (midi[i][j].isNote()) {
                int channel = midi[i][j].getChannelNibble();
                if (channel == 9) {
                    midi[i][j].clear();
                }
            }
        }
    }

    midi.removeEmpties();
}
void Piano::loadMidi(const char* file) {
    smf::MidiFile midi;
    midi.read(file);
    if (!midi.status()) {
        std::cerr << "Problem reading MIDI file\n";
        return;
    }
    removePercussionNotes(midi); // should be good enough

    midi.doTimeAnalysis();
    midi.linkNotePairs();
    midi.joinTracks();
    midi.sortTracks(); // might not be needed

    this->loaded_song = midi;
}



void playText(const Piano&, const std::vector<Note>&);
void playMidi(const Piano&, const smf::MidiFile&);

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void Piano::play() {
    if (this->loaded_song.valueless_by_exception()) {
        std::cerr << "No loaded song\n";
        return;
    }

    std::visit(overloaded {
        [this](const std::vector<Note>& notes) {
            playText(*this, notes);
        },
        [this](const smf::MidiFile& midi) {
            playMidi(*this, midi);
        }
    }, this->loaded_song);

}

void playText(const Piano& piano, const std::vector<Note>& notes) {
    {
        std::unique_ptr<ShiftGuard> optShift;
        for (auto &note : notes) {
            playNote(piano, note, optShift);
            Sleep(note.delay); // usually NOTE_LENGTH
        }
    }
}

void playMidi(const Piano& piano, const smf::MidiFile& midi) {
    const auto midiTo61 = [](int midiKey) -> int { // may be out of range
        return midiKey - 35;
    };
    const auto& track = midi[0];

    std::unique_ptr<ShiftGuard> shift_guard;
    for (int eventIdx = 0; eventIdx < track.size(); eventIdx++) {
        const auto& event = track[eventIdx];
        if (event.isNoteOn()) {

            const int keyNum = std::clamp(midiTo61(event.getKeyNumber()), 1, 61);
            const char key = KEYNUM_TO_KEY.at(keyNum);

            playKey(piano, key, shift_guard);

            if (eventIdx < track.size() - 1) { // if not last note
                const auto& next = [&]{
                    for (int i = eventIdx + 1; i < track.size(); i++) {
                        if (track[i].isNoteOn()) return track[i];
                    }
                    return track[eventIdx]; // TODO: handle this correctly
                }();
                const double timeDiff = next.seconds - event.seconds;
                const int timeDiffMillis = timeDiff * 1000.0;
                Sleep(timeDiffMillis);
            }
        }
    }
}

