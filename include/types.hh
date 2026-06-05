#ifndef _TYPES_HH_
#define _TYPES_HH_

#include <cstdint>
#include <vector>
#include <iostream>

#define TPRINTF(statement) std::cout << statement << std::endl;
#define EPRINTF(error) std::cerr << error;

#define Cycles uint64_t
#define Addr uint16_t
#define ProgramCounter uint16_t
#define Instruction uint16_t
#define OpCode uint8_t
#define Color uint8_t
#define Pixel uint8_t
#define Byte int8_t
#define Register int8_t
#define RegisterTable std::vector<Register>

#endif // _TYPES_HH_