#include <iostream>

#include "Display/video.hh"

int main() {
    std:: cout << "Hello, Chip-8!" << std::endl;

    chip8::display::Video display;
    
    display.run();

    return 0;
}