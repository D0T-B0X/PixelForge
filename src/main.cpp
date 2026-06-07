#include <iostream>

#include "system.hh"

int main(int argc, char* argv[]) {
    TPRINTF("Hello, Chip-8!")

    // Initialize system subcomponents;
    chip8::System system;
    
    // run the binary on the VM
    system.run();

    return 0;
}