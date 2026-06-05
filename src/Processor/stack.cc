#include "Processor/stack.hh"

namespace chip8
{

namespace processor
{

Stack::Stack()
 : sp(0)
{
    // empty the stack
    for(int i = 0; i < 16; ++i) stack[i] = 0;
}

void
Stack::push(ProgramCounter pc)
{
    // terminate the program on a stack overflow
    if (sp == 15) {
        EPRINTF("FATAL: Stack Overflow.\n")
        std::exit(-1);
    }

    stack[sp++] = pc;
}

ProgramCounter
Stack::pop()
{
    // terminate the program on a stack underflow
    if (sp == 0) {
        EPRINTF("FATAL: Stack underflow.\n");
        std::exit(-1);
    }

    return stack[--sp];
}

} // namespace processor 

} // namespace chip8
