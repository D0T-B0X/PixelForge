#ifndef _UTIL_ROM_LOADER_HH_
#define _UTIL_ROM_LOADER_HH_

#include <fstream>
#include <string>

#include "Memory/ram.hh"

#define MAX_FILE_SIZE 3584 // (4096 - 512) bytes

namespace chip8
{

namespace util
{

class RomLoader
{
  public:
    RomLoader();

    /**
     * @brief Load the uploaded rom and parse it
     * 
     * @param path The location of the game ROM
     */
    std::vector<Byte> parseROM(const char* path);
};

} // namespace util

} // namespace chip8

#endif // _UTIL_ROM_LOADER_HH_