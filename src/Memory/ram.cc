#include "Memory/ram.hh"

namespace chip8
{

namespace memory
{

RAM::RAM()
{
    // reset RAM to 0
    for(int i = 0; i < 4096; ++i) ram[i] = 0;
}

Byte
RAM::load(Addr address)
{
    // access over 4095 and below 512 results in termination 
    if (address > 0xFFF || address < 0x200) {
        std::cout << "FATAL: RAM::Invalid Address Access" << std::endl;
        exit(-1);
    }

    // return data from valid address
    return ram[address];
}

void
RAM::store(Addr address, Byte data)
{
    // access over 4095 and below 512 results in termination 
    if (address > 0xFFF || address < 0x200) {
        std::cout << "FATAL: RAM::Invalid Address Access" << std::endl;
        exit(-1);
    }

    ram[address] = data;
}

void
RAM::loadRom(std::vector<Byte>& binary)
{
    // Always load ROM into RAM from 0x200 (512 B)
    std::copy(binary.begin(), binary.end(), &ram[0x200]);
}

} // namespace memory

} // namespace chip8