#include <array>
#include <iostream>
#include <exception>

#include <Windows.h>

#include "Piano.h"
//#include "MidiFile.h"

#include <boost/program_options.hpp>


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

        try {
            piano.load(argv[1]);
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