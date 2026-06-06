#ifndef _PROCESSOR_CPU_HH_
#define _PROCESSOR_CPU_HH_

#include <vector>

#include "Display/video.hh"
#include "IO/keypad.hh" 
#include "Memory/ram.hh"
#include "Processor/stack.hh"
#include "Timer/delay.hh"
#include "Timer/sound.hh"
#include "types.hh"

namespace chip8
{

namespace processor
{

struct Operands {
    OpCode op_code; // First nibble
    OpCode sub_op_code; // Last nibble
    Register x; //  Second nibble
    Register y; // Third nibble
    Addr address; // NNN
    Byte data; // NN
};

class CPU
{
  private:
    
    // Total number of clock cycles
    Cycles cycle_count;

    // Program Counter
    ProgramCounter pc;

    // Memory Index Pointer
    Addr i;

    // General Purpose registers
    RegisterTable register_table;

    // Reference to RAM object for memory access
    memory::RAM& ram;

    // Reference to the Video object for display access
    display::Video& display;

    // Reference to the delay and sound timer objects
    timer::Delay& delay;
    timer::Sound& sound;

    // The CPU call stack
    processor::Stack stack;

    // Keypad io object reference
    io::Keypad& keypad;

    /**
     * @brief Fetches the next instruction in the binary
     * 
     * @return The fetched instruction
     */
    Instruction fetch();

    /**
     * @brief Decode the instruction into its operands
     * 
     * @param instruction The instruction to decode
     * @return The operands of the decoded instruction
     */
    Operands decode(const Instruction& instruction);

    /**
     * @brief 
     * 
     * @param ops 
     */
    void execute(const Operands& ops);

    /*************
     * Instructions are divided into 5 categories
     *
     * 1) Control: Unconditional system operations 
     * 2) Conditional Branching: Branching operations dependent on computed values
     * 3) ALU : ALU operations (addition, sub, bit shift etc)
     * 4) Memory/Display: RAM and display related operations
     * 5) Timers and IO : Keypad input and Sound/Delay timer instructions
     *   
     */

    /**
    * @brief Control flow operation execution(pc jumps, display clear)
    * 
    * @param ops Operands of the decoded instruction
    */
    void execute_ControlOperations(const Operands& ops);

    /**
     * @brief Condition branch operation exection
     * 
     * @param ops Operands of the decoded instruction
     */
    void execute_BranchOperations(const Operands& ops);

    /**
     * @brief Math and logic operation execution
     * 
     * @param ops Operands of the decoded instruction
     */
    void execute_ALUOperations(const Operands& ops);

    /**
     * @brief Direct Memory access and sprite draw operations
     * 
     * @param ops Operands of the decoded instruction
     */
    void execute_MemoryDisplayOperations(const Operands& ops);

    /**
     * @brief Timer and keypad input operations
     * 
     * @param ops Operands of the decoded instruction
     */
    void execute_IOOperations(const Operands& ops);

    // *** ISA methods ***

    // 0x00E0 maps to display clear
    void I_00E0();

    // 0x00EE Pop the call stack and return the pc
    void I_00EE();

    // Unconditional jump to memory address 0xNNN
    void I_1NNN(const Operands& ops);

    // Call a subroutine at 0xNNN
    void I_2NNN(const Operands& ops);

    // Skip the next instruction if VX == NN
    void I_3XNN(const Operands& ops);

    // Skip the next instruction if VX != NN
    void I_4XNN(const Operands& ops);

    // Skip the next instruction if VX == VY
    void I_5XY0(const Operands& ops);

    // Set VX = NN
    void I_6XNN(const Operands& ops);

    // Set VX = VX + NN
    void I_7XNN(const Operands& ops);

    // Set VX = VY;
    void I_8XY0(const Operands& ops);

    // Set VX = VX | VY
    void I_8XY1(const Operands& ops);

    // Set VX = VX & VY
    void I_8XY2(const Operands& ops);

    // Set VX = VX ^ VY
    void I_8XY3(const Operands& ops);

    // Set VX = VX + VY
    void I_8XY4(const Operands& ops);

    // Set VX = VX - VY
    void I_8XY5(const Operands& ops);

    // Set VX = VX >> VY
    void I_8XY6(const Operands& ops);

    // Set VX = VY - VX
    void I_8XY7(const Operands& ops);

    // Set VX = VX << VY
    void I_8XYE(const Operands& ops);

    // Skip the next instruction if VX != VY
    void I_9XY0(const Operands& ops);

    // Set Memory index (i) = NNN
    void I_ANNN(const Operands& ops);

    // Jump to memory address (V0 + NNN)
    void I_BNNN(const Operands& ops);

    // Set VX = Random(0, 255) & NN
    void I_CXNN(const Operands& ops);

    // Sprite VX VY N
    void I_DXYN(const Operands& ops);

    // Skip if key is pressed
    void I_EX9E(const Operands& ops);

    // Skip if key is not pressed
    void I_EXA1(const Operands& ops);

    // Set VX = delay timer
    void I_FX07(const Operands& ops);

    // Set VX = key
    void I_FX0A(const Operands& ops);

    // delay timer = VX
    void I_FX15(const Operands& ops);

    // buzzer = VX;
    void I_FX18(const Operands& ops);

    // Set i = i + VX;
    void I_FX1E(const Operands& ops);

    // Set i = hex VX
    void I_FX29(const Operands& ops);

    // Decode VX into BCD
    void I_FX33(const Operands& ops);

    // save (V0 - VF) in memory addresses (i) to (i + x)
    void I_FX55(const Operands& ops);

    // load (V0 - VF) from memory addresses (i) to (i + x)
    void I_FX65(const Operands& ops);

  public:
    CPU(
      memory::RAM& mem, 
      display::Video& display,
      timer::Delay& delay,
      timer::Sound& sound,
      io::Keypad& keypad
    );
    
    void cycle();
};

} // namespace processor

} // processor chip8

#endif // _PROCESSOR_CPU_HH_
