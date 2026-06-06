#include "Processor/cpu.hh"

namespace chip8
{

namespace processor
{

// PC always starts from 0x200
// The memory index pointer is 
CPU::CPU(
    memory::RAM& mem, 
    display::Video& display,
    timer::Delay& delay,
    timer::Sound& sound
)
 : pc(0x200), cycle_count(0), ram(mem), 
   display(display), delay(delay),
   sound(sound)
{
    // 16 general purpose registers (V0 - VF)
    register_table = RegisterTable(16); 
}

Instruction
CPU::fetch()
{
    // Load the upper and lower bytes of the instruction
    // and increment the PC
    Byte upper = ram.load(pc++);
    Byte lower = ram.load(pc++);

    Instruction inst = (upper << 8) | lower;

    return inst;
}

Operands
CPU::decode(const Instruction& instruction)
{   
    Operands ops;

    ops.op_code      =    (instruction & 0xF000) >> 12; // First 4 bits
    ops.x            =    (instruction & 0x0F00) >> 8; // 2nd nibble
    ops.y            =    (instruction & 0x00F0) >> 4; // 3rd nibble
    ops.sub_op_code  =    (instruction & 0x000F); // 4th nibble (or N for DXYN)
    ops.address      =    (instruction & 0x0FFF); // NNN memory address
    ops.data         =    (instruction & 0x00FF); // NN data byte

    return ops;
}

void
CPU::execute(const Operands& ops)
{
    OpCode op_code = ops.op_code;

    // Route the operands to the correct category of the instruction
    // Any Op Code outside 0x0-F is impossible
    if (op_code < 0x3) 

        // control op codes: 0x0..., 0x1..., 0x2...
        execute_ControlOperations(ops); 
    else if (op_code < 0x6 || op_code == 0x9 || op_code == 0xE)
    
        // conditonal branch op codes: 0x3..., 0x4..., 0x5..., 0x9..., 0xE...  
        execute_BranchOperations(ops); 
    else if (op_code < 0x9) 
        
        // ALU op codes: 0x6, 0x7, 0x8
        execute_ALUOperations(ops); 
    else if (op_code < 0xE)
        
        // Memory/Display op codes: 0xA, 0xB, 0xC, 0xD
        execute_MemoryDisplayOperations(ops);
    else
        
        // IO op codes: 0xF
        execute_IOOperations(ops);
}

void
CPU::execute_ControlOperations(const Operands& ops)
{
    switch (ops.op_code) 
    {
        case 0x0:
            switch (ops.sub_op_code) 
            {
                case 0x0:
                    I_00E0();
                    break;
                case 0xE:
                    I_00EE();
                    break;
                default:
                    EPRINTF("FATAL: Unrecognized instruction.\n")
                    std::exit(-1);
            }
            break;
        case 0x1:
            I_1NNN(ops);
            break;
        case 0x2:
            I_2NNN(ops);
            break;
    }
}

void
CPU:: execute_BranchOperations(const Operands& ops)
{
    switch (ops.op_code)
    {
        case 0x3:
            I_3XNN(ops);
            break;
        case 0x4:
            I_4XNN(ops);
            break;
        case 0x5:
            I_5XY0(ops);
            break;
        case 0x9:
            I_9XY0(ops);
            break;
        case 0xE:
            switch (ops.sub_op_code)
            {
                case 0xE:
                    I_EX9E(ops);
                    break;
                case 0x1:
                    I_EXA1(ops);
                    break;
            }
            break;
    }
}

void
CPU::execute_ALUOperations(const Operands& ops)
{
    switch (ops.op_code)
    {
        case 0x6:
            I_6XNN(ops);
            break;
        case 0x7:
            I_7XNN(ops);
            break;
        case 0x8:
            switch(ops.sub_op_code)
            {
                case 0x0:
                    I_8XY0(ops);
                    break;
                case 0x1:
                    I_8XY1(ops);
                    break;
                case 0x2:
                    I_8XY2(ops);
                    break;
                case 0x3:
                    I_8XY3(ops);
                    break;
                case 0x4:
                    I_8XY4(ops);
                    break;
                case 0x5:
                    I_8XY5(ops);
                    break;
                case 0x6:
                    I_8XY6(ops);
                    break;
                case 0x7:
                    I_8XY7(ops);
                    break;
                case 0xE:
                    I_8XYE(ops);
                    break;
                default:
                    EPRINTF("FATAL: Unrecognized instruction.\n")
                    std::exit(-1);
            }
    }
}

void
CPU::execute_MemoryDisplayOperations(const Operands& ops)
{
    switch (ops.op_code)
    {
        case 0xA:
            I_ANNN(ops);
            break;
        case 0xB:
            I_BNNN(ops);
            break;
        case 0xC:
            I_CXNN(ops);
            break;
        case 0xD:
            I_DXYN(ops);
            break;
    }
}

void
CPU::execute_IOOperations(const Operands& ops)
{
    // since all IO operations use the 'F' op code, we 
    // need to make a decision based on the last byte
    switch (ops.data)
    {
        case 0x07:
            I_FX07(ops);
            break;
        case 0x0A:
            I_FX0A(ops);
            break;
        case 0x15:
            I_FX15(ops);
            break;
        case 0x18:
            I_FX18(ops);
            break;
        case 0x1E:
            I_FX1E(ops);
            break;
        case 0x29:
            I_FX29(ops);
            break;
        case 0x33:
            I_FX33(ops);
            break;
        case 0x55:
            I_FX55(ops);
            break;
        case 0x65:
            I_FX65(ops);
            break;
        default:
            EPRINTF("FATAL: Unrecognized instruction.\n");
            std::exit(-1);
    }
}

// clear the display
void
CPU::I_00E0()
{
    display.clear();
}

// pop call stack and return pc
void
CPU::I_00EE()
{
    pc = stack.pop();
}

// unconditional jump to NNN
void
CPU::I_1NNN(const Operands& ops)
{
    pc = ops.address;
}

// call subroutine at NNN
void
CPU::I_2NNN(const Operands& ops)
{
    stack.push(pc);
    pc = ops.address;
}

// skip the next instruction if VX == NN
void
CPU::I_3XNN(const Operands& ops)
{
    if (register_table[ops.x] == ops.data) {
        pc += 2;
    }
}

// skip the next instruction if VX != NN
void
CPU::I_4XNN(const Operands& ops)
{
    if (register_table[ops.x] != ops.data) {
        pc += 2;
    }
}

// skip the next instruction if VX == VY
void
CPU::I_5XY0(const Operands& ops)
{
    if (register_table[ops.x] == register_table[ops.y]) {
        pc += 2;
    }
}

// skip the next instruction if VX != VY
void
CPU::I_9XY0(const Operands& ops)
{
    if (register_table[ops.x] != register_table[ops.y]) {
        pc += 2;
    }
}

// skip the next instruction if a key press is detected
void
CPU::I_EX9E(const Operands& ops)
{
    // if (keyboard.isPressed(register_table[ops.x])) {
    //    pc += 2;
    // }
}

// skip the next instruction if a key is not pressed
void
CPU::I_EXA1(const Operands& ops)
{
    // if (!keyboard.isPressed(register_table[ops.x])) {
    //    pc += 2;
    // }
}

// immediate value assignment to a register
void
CPU::I_6XNN(const Operands& ops)
{
    register_table[ops.x] = ops.data;
}

// immediate value addition to a register
void
CPU::I_7XNN(const Operands& ops)
{
    register_table[ops.x] += ops.data;
}

// register value assignment to a register
void
CPU::I_8XY0(const Operands& ops)
{
    register_table[ops.x] = register_table[ops.y];
}

// bitwise OR between two registers
void
CPU::I_8XY1(const Operands& ops)
{
    register_table[ops.x] |= register_table[ops.y];
}

// bitwise AND between two registers
void
CPU::I_8XY2(const Operands& ops)
{
    register_table[ops.x] &= register_table[ops.y];
}

// bitwise xor between two registers
void
CPU::I_8XY3(const Operands& ops)
{
    register_table[ops.x] ^= register_table[ops.y];
}

// register addition and carry flag set
void
CPU::I_8XY4(const Operands& ops)
{
    uint16_t sum = register_table[ops.x] + register_table[ops.y];
    register_table[ops.x] = sum & 0xFF;
    register_table[0xF] = sum > 0xFF;
}

// register subtraction and borrow flag reset
void 
CPU::I_8XY5(const Operands& ops)
{
    Register x = register_table[ops.x];
    Register y = register_table[ops.y];
    register_table[ops.x] = x - y;
    register_table[0xF] = x >= y;
}

// right shift with lsb in flag 
void
CPU::I_8XY6(const Operands& ops)
{
    Register lsb = register_table[ops.y] & 0x01;
    register_table[ops.x] = register_table[ops.y] >> 1;
    register_table[0xF] = lsb;
}

// inverted register subtraction and borrow set
void
CPU::I_8XY7(const Operands& ops)
{
    Register x = register_table[ops.x];
    Register y = register_table[ops.y];
    register_table[ops.x] = y - x;
    register_table[0xF] = (y >= x) ? 1 : 0;
}

// left shift with msb in flag 
void
CPU::I_8XYE(const Operands& ops)
{
    Register msb = (register_table[ops.y] & 0x80) >> 7;
    register_table[ops.x] = register_table[ops.y] << 1;
    register_table[0xF] = msb;
}

// set index register I to NNN
void
CPU::I_ANNN(const Operands& ops)
{
    i = ops.address;
}

// jump to address NNN + V0
void
CPU::I_BNNN(const Operands& ops)
{
    pc = ops.address + register_table[0x0];
}

// set VX to random byte bitwise ANDed with NN
void
CPU::I_CXNN(const Operands& ops)
{
    Byte random_byte = std::rand() % 256; 
    register_table[ops.x] = random_byte & ops.data;
}

// display sprite at (VX, VY) with width 8 and height N
void
CPU::I_DXYN(const Operands& ops)
{
    Byte x_coord = register_table[ops.x] % 64; 
    Byte y_coord = register_table[ops.y] % 32; 
    Byte height = ops.sub_op_code;

    register_table[0xF] = 0;

    for (Byte row = 0; row < height; ++row) {
        Byte sprite_byte = ram.load(i + row);
        
        for (Byte col = 0; col < 8; ++col) {
            Byte sprite_pixel = sprite_byte & (0x80 >> col);
            
            if (sprite_pixel) {
                if (display.setPixel(x_coord + col, y_coord + row)) {
                    register_table[0xF] = 1; 
                }
            }
        }
    }
}

// set VX to delay timer value
void
CPU::I_FX07(const Operands& ops)
{
    register_table[ops.x] = delay.getTimer();
}

// await key press and store in VX (blocking instruction)
void
CPU::I_FX0A(const Operands& ops)
{
    Byte key = 1; // keyboard.getPressedKey(); 
    if (key == -1) {
        pc -= 2; 
    } else {
        register_table[ops.x] = key;
    }
}

// set delay timer to VX
void
CPU::I_FX15(const Operands& ops)
{
    delay.setTimer(register_table[ops.x]);
}

// set sound timer to VX
void
CPU::I_FX18(const Operands& ops)
{
    sound.setTimer(register_table[ops.x]);    
}

// add VX to index register I
void
CPU::I_FX1E(const Operands& ops)
{
    i += register_table[ops.x];
}

// set I to the location of the sprite for the character in VX
void
CPU::I_FX29(const Operands& ops)
{
    Byte character_index = register_table[ops.x] & 0x0F;
    
    // get the location of the character
    i = FONTSET_START_ADDRESS + (character_index * FONT_CHARACTER_SIZE); 
}

// store binary-coded decimal representation of VX at I, I+1, I+2
void
CPU::I_FX33(const Operands& ops)
{
    Byte x = register_table[ops.x];
    ram.store(i, x / 100);
    ram.store(i + 1, (x / 10) % 10);
    ram.store(i + 2, x % 10);
}

// store registers V0 through VX in memory starting at I
void
CPU::I_FX55(const Operands& ops)
{
    for (Register index = 0; index <= ops.x; ++index) {
        ram.store(i + index, register_table[index]);
    }
}

// load registers V0 through VX from memory starting at I
void
CPU::I_FX65(const Operands& ops)
{
    for (Register index = 0; index <= ops.x; ++index) {
        register_table[index] = ram.load(i + index);
    }
}

void
CPU::cycle()
{
    // Fetch Instruction bytes
    Instruction inst = fetch();

    // Decode recieved instruction
    Operands ops = decode(inst);

    // execute the instruction
    execute(ops);
}

} // namespace processor

} // namespace chip8