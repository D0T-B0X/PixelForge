#ifndef _PROCESSOR_STACK_HH_
#define _PROCESSOR_STACK_HH_

#include <cstdint>

#include "types.hh"

namespace chip8
{

namespace processor
{

class Stack
{
  private:
    // 16 level stack for the system
    chip8::Addr stack[16];

    // stack pointer
    chip8::Byte sp;

  public:

    /**
     * 
     */
    void push();

    /**
     * 
     */
    chip8::Addr pop();
};

} // namespace processor

} // namespace chip8 

#endif // _PROCESSOR_STACK_HH_