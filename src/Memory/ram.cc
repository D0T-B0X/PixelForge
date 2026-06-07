#include "Memory/ram.hh"

namespace chip8
{

namespace memory
{

RAM::RAM()
{
    // load the fontset into RAM
    std::copy(
        std::begin(display::fontset),
        std::end(display::fontset),
        &ram[FONTSET_START_ADDRESS]
    );
}

Byte
RAM::load(Addr address) const
{  
    // Access over 4095 results in termination
    if (address > 0xFFF) {
        EPRINTF("FATAL: RAM::Load::Invalid Address Access.\n")
        exit(-1);
    }

    // return data from valid address
    return ram[address];
}

void
RAM::store(Addr address, Byte data)
{
    // access over 4095 results in termination
    if (address > 0xFFF) {
        EPRINTF("FATAL: RAM::Store::Invalid Address Access.\n")
        exit(-1);
    }

    ram[address] = data;
}

void
RAM::loadRom(const std::vector<Byte>& binary)
{
    // Always load ROM into RAM from 0x200 (512 B)
    std::copy(
        binary.begin(), 
        binary.end(), 
        &ram[0x200]
    );
}

} // namespace memory

} // namespace chip8