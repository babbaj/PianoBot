#include <array>
#include <iostream>

#include <Windows.h>

#include "Piano.h"
//#include "MidiFile.h"


int main(const int argc, const char* argv[]) {
    if (argc < 2) {
        puts("Usage: <input file>");
        //return 1;
    }

    HWND hWindowHandle = FindWindow(nullptr, "Garry's Mod"); // TODO: find by process name
    if (!hWindowHandle) {
        std::cerr << "Failed to acquire handle\n";
        return 1;
    }
    Piano piano(hWindowHandle);

    try {
        piano.load("song.txt");
    } catch(std::ifstream::failure &ex) {
        std::cerr << "Failed to read file\n";
        std::cerr << ex.what() << '\n'; // not very useful
        return 1;
    }

    //while (1) {
        piano.play();
        //Sleep(1000);
    //}


    return 0;
}