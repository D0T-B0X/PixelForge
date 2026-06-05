#ifndef _PROCESSOR_CPU_HH_
#define _PROCESSOR_CPU_HH_

#include <vector>

#include "types.hh"

namespace chip8
{

namespace processor
{

struct Instruction {
    OpCode op_code;
    Addr address;
    Byte data;
};

class CPU
{
  private:
    
    // Total number of clock cycles
    Cycles cycle_count;

    // Program Counter
    Register pc;

    // General Purpose registers
    RegisterTable register_table;

    void fetch();

    void decode();

    void execute();

  public:
    CPU();
    
    void cycle();
};

} // namespace processor

} // processor chip8

#endif // _PROCESSOR_CPU_HH_