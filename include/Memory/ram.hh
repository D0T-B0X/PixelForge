#ifndef _MEMORY_RAM_HH_
#define _MEMORY_RAM_HH_

#include <cstdint>

#include "types.hh"

namespace chip8
{

namespace memory
{

class RAM
{
  private: 
    Byte ram[4096];

  public:
    RAM();

    Byte load(Addr address);

    void store(Addr address, Byte data);
    
};

} // namespace memory

} // namespace chip8

#endif // _MEMORY_RAM_HH_