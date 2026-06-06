#include "Util/rom_loader.hh"

namespace chip8
{

namespace util

{

RomLoader::RomLoader()
{
}

std::vector<Byte>
RomLoader::parseROM(const char* path)
{
    // load the file into a file stream as a binary 
    // and immediately move to the end of the file
    std::ifstream binary(path, std::ios::binary | std::ios::ate);

    // fatal: program can't proceed if the binary is invalid
    if (!binary.is_open()) {
        EPRINTF("FATAL: Could not open ROM.\n")
        std::exit(-1);
    }

    // size of the binary
    std::streampos size = binary.tellg();

    // move read cursor back to the beginning
    binary.seekg(0, std::ios::beg);

    if (size > MAX_FILE_SIZE) {
        EPRINTF("FATAL: ROM is too large for memory.\n")
        std::exit(-1);
    }

    std::vector<Byte> binaryData(size);

    if ( !binary.read(reinterpret_cast<char*>(binaryData.data()), size) ) {
        EPRINTF("FATAL: Failed to read ROM.\n")
        std::exit(-1);
    }

    return binaryData;
}

} // namespace util
} // namespace chip8