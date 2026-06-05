#include <iostream>

#include "Display/video.hh"

int main() {
    TPRINTF("Hello, Chip-8!")

    chip8::display::Video display;
    
    display.run();

    return 0;
}