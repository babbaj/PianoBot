#include <array>
#include <iostream>
#include <exception>
#include <iomanip>

#include <Windows.h>

#include "Piano.h"

#include "MidiFile.h"
#include "Options.h"

#include <boost/program_options.hpp>


bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}



int main(const int argc, const char* argv[]) {
    try {
        namespace po = boost::program_options;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("loop", "Play song on repeat");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (argc < 2 || vm.count("help")) {
            puts("Usage: <input file>");
            std::cout << desc << "\n";
            return 1;
        }


        HWND hWindowHandle = FindWindow(nullptr, "Garry's Mod"); // TODO: find by process name
        if (!hWindowHandle) {
            std::cerr << "Failed to acquire handle\n";
            return 1;
        }
        Piano piano(hWindowHandle);

        const std::string fileName = argv[1];
        try {
            if (hasEnding(fileName, ".mid")) {
                piano.loadMidi(fileName.c_str());
            } else {
                piano.loadText(fileName.c_str());
            }
        } catch (std::ifstream::failure &ex) {
            std::cerr << "Failed to read file\n";
            std::cerr << ex.what() << '\n'; // not very useful
            return 1;
        }


        const bool loop = vm.count("loop");
        do {
            piano.play();
            Sleep(1000);
        } while (loop);

    } catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        return 1;
    }

    return 0;
}