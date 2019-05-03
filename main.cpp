#include <array>
#include <iostream>

#include <Windows.h>

#include "Piano.h"

//constexpr auto SHIFT_LPARAM = 0x002C0001; // includes scancode



int main(const int argc, const char* argv[]) {
    if (argc < 2) {
        puts("Usage: <input file>");
        //return 1;
    }

    HWND hWindowHandle = FindWindow(nullptr, "Garry's Mod"); // TODO: find by process name
    Piano piano(hWindowHandle);

    try {
        piano.play("coldplay.txt");
    } catch(std::ifstream::failure &ex) {
        std::cerr << "Failed to read file\n";
        std::cerr << ex.what() << '\n';
    }

    //SendMessage(hWindowHandle, WM_KEYDOWN, VK_SHIFT, SHIFT_LPARAM);

    /*while (1) {
        for (auto key : COLDPLAY) {
         SendMessage(hWindowHandle, WM_KEYDOWN, key, 0x1);
         Sleep(5);
         SendMessage(hWindowHandle, WM_KEYUP, key, 0x1);

         Sleep(200);
        }
    }*/

    //SendMessage(hWindowHandle, WM_KEYUP, VK_SHIFT, SHIFT_LPARAM);
    return 0;
}