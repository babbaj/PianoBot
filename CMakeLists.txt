cmake_minimum_required(VERSION 3.13)
project(GmodPianoPlayer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(midifile)

get_filename_component(MIDI_DIR midifile/include ABSOLUTE)

include_directories(${MIDI_DIR})

file(GLOB MIDI_SRC midifile/src/*.cpp)


add_executable(GmodPianoPlayer
        main.cpp

        Note.h
        Note.cpp

        ShiftGuard.h

        Piano.cpp
        Piano.h

        Keys.h

        ${MIDI_SRC})