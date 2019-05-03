cmake_minimum_required(VERSION 3.13)
project(GmodPianoPlayer)

set(CMAKE_CXX_STANDARD 17)

add_executable(GmodPianoPlayer main.cpp Note.h Note.cpp ShiftGuard.h Piano.cpp Piano.h Keys.h)