cmake_minimum_required(VERSION 3.13)
project(GmodPianoPlayer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(midifile)
add_subdirectory(boost-cmake)


get_filename_component(MIDI_DIR midifile/include ABSOLUTE)

include_directories(${MIDI_DIR})

file(GLOB MIDI_SRC midifile/src/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(GmodPianoPlayer
        main.cpp

        Note.h

        ShiftGuard.h

        Piano.cpp
        Piano.h

        Keys.h

        ${MIDI_SRC})

target_link_libraries(GmodPianoPlayer PUBLIC Boost::program_options)
#target_link_libraries(GmodPianoPlayer PUBLIC Boost::filesystem)

