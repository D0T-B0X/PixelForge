#ifndef _MEMORY_RAM_HH_
#define _MEMORY_RAM_HH_

#include <cstdint>

#include "Display/fontset.hh"
#include "types.hh"

namespace chip8
{

namespace memory
{

class RAM
{
  private: 
    // 4 KB of RAM
    Byte ram[4096] = {0};

  public:
    RAM();

    /**
     * @brief Load a byte from RAM
     * 
     * @param address 
      * @return Byte 
     */
        Byte load(Addr address) const;

    /**
     * @brief Store a byte in RAM
     * 
     * @param address Where the byte needs to be stored in RAM
     * @param data The actual byte of data
     */
    void store(Addr address, Byte byte);

    /**
     * @brief Load ROM data into the RAM
     * 
     * @param binary A vector holding the binary ROM data 
     */
    void loadRom(std::vector<Byte>& binary);
    
};

} // namespace memory

} // namespace chip8

#endif // _MEMORY_RAM_HH_