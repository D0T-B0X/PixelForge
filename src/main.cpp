#include <iostream>

#include "system.hh"

int main() {
    TPRINTF("Hello, Chip-8!")

    // Initialize system subcomponents;
    chip8::System system;
    
    system.run();

    return 0;
}