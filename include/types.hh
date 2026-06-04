#ifndef _TYPES_HH_
#define _TYPES_HH_

#include <cstdint>
#include <vector>
#include <iostream>

#define TPRINTF(statement) std::cout << statement << std::endl;

namespace chip8
{

typedef uint64_t Cycles;
typedef uint16_t Addr;
typedef uint16_t OpCode;
typedef uint8_t Color;
typedef uint8_t Pixel;
typedef int8_t Byte;
typedef int8_t Register;
typedef std::vector<Register> RegisterTable;

} // namespace chip8

#endif // _TYPES_HH_