#include <iostream>

#include "system.hh"

int main(int argc, char* argv[]) {
    TPRINTF("Hello, Chip-8!")

    // Initialize system subcomponents;
    chip8::System system;

    // Parse command line arguments to get the binary path 
    if (argc != 2) {
        EPRINTF("FATAL: Invalid argument list.\n");
        std::exit(-1);
    }

    std::string romPath = argv[1];

    // Load the binary into Chip8 memory
    system.loadRom(romPath.c_str());
    
    // run the binary on the VM
    system.run();

    return 0;
}