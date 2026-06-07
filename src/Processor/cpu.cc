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
    timer::Sound& sound,
    io::Keypad& keypad,
    EmulatorQuirks& quirks
)
 : pc(0x200), cycle_count(0), ram(mem), 
   display(display), delay(delay),
   sound(sound), keypad(keypad), quirks(quirks)
{
    // 16 general purpose registers (V0 - VF)
    register_table = RegisterTable(16); 

    for(int i = 0; i < 8; ++i) rpl_flags[i] = 0;
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
            switch (ops.y) // differentiate instructions in 0x00 based on 3rd nibble
            {
                case 0xC:
                    I_00CN(ops);
                    break;
                case 0xE:
                    switch(ops.sub_op_code) { // differentiate final instruction based on sub op code
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
                case 0xF:
                    switch(ops.sub_op_code) { // differentiate final instruction based on sub op code
                        case 0xB:
                            I_00FB();
                            break;
                        case 0xC:
                            I_00FC();
                            break;
                        case 0xD:
                            I_00FD();
                            break;
                        case 0xE:
                            I_00FE();
                            break;
                        case 0xF: 
                            I_00FF();
                            break;
                        default:
                            EPRINTF("Unrecognized instruction.\n")
                            std::exit(-1);
                    }
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

// 0x00CN: Scroll display down by N lines
void 
CPU::I_00CN(const Operands& ops)
{
    // The 'N' value is stored in the sub_op_code (last nibble)
    display.scrollDown(ops.sub_op_code); 
}

// 0x00FB: Scroll display right by 4 pixels
void 
CPU::I_00FB()
{
    display.scrollRight();
}

// 0x00FC: Scroll display left by 4 pixels
void 
CPU::I_00FC()
{
    display.scrollLeft();
}

// 0x00FD: Halt emulator execution
void 
CPU::I_00FD()
{
    TPRINTF("Emulator halted ")
}

// 0x00FE: Disable high-resolution mode (Standard CHIP-8 mode)
void 
CPU::I_00FE()
{
    display.setLowRes();
}

// 0x00FF: Enable high-resolution mode (SCHIP mode)
void 
CPU::I_00FF()
{
    display.setHighRes();
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
    if (keypad.isKeyPressed(register_table[ops.x])) {
       pc += 2;
    }
}

// skip the next instruction if a key is not pressed
void
CPU::I_EXA1(const Operands& ops)
{
    if (!keypad.isKeyPressed(register_table[ops.x])) {
       pc += 2;
    }
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
if (quirks.shift_modifies_vx_in_place) {
        Register lsb = register_table[ops.x] & 0x01;
        register_table[ops.x] >>= 1;
        register_table[0xF] = lsb;
    } else {
        Register lsb = register_table[ops.y] & 0x01;
        register_table[ops.x] = register_table[ops.y] >> 1;
        register_table[0xF] = lsb;
    }
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
    if (quirks.shift_modifies_vx_in_place) {
        Register msb = (register_table[ops.x] & 0x80) >> 7;
        register_table[ops.x] <<= 1;
        register_table[0xF] = msb;
    } else {
        Register msb = (register_table[ops.y] & 0x80) >> 7;
        register_table[ops.x] = register_table[ops.y] << 1;
        register_table[0xF] = msb;
    }
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
if (quirks.jump_with_offset_vx) {
        pc = ops.address + register_table[ops.x];
    } else {
        pc = ops.address + register_table[0x0];
    }
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
    Byte x_pos = register_table[ops.x];
    Byte y_pos = register_table[ops.y];
    Byte height = ops.sub_op_code; // N value

    // Reset collision flag
    register_table[0xF] = 0;

    Byte max_x = display.getWidth();
    Byte max_y = display.getHeight();

    if (height == 0) { // SCHIP 16x16 sprite mode
        for (int row = 0; row < 16; ++row) {
            // Fetch 2 consecutive bytes for a 16-bit row
            uint16_t sprite_row = (ram.load(i + (row * 2)) << 8) | ram.load(i + (row * 2) + 1);

            for (int col = 0; col < 16; ++col) {
                // Evaluate each bit from MSB to LSB
                if ((sprite_row & (0x8000 >> col)) != 0) {
                    Byte target_x = x_pos + col;
                    Byte target_y = y_pos + row;

                    // Execute SCHIP display clipping quirk
                    if (quirks.clip_sprites_at_edges) {
                        if (target_x >= max_x || target_y >= max_y) {
                            continue; 
                        }
                    }

                    // setPixel returns true if a collision was detected, set VF as setPixel return
                    if (display.setPixel(target_x, target_y)) {
                        register_table[0xF] = 1;
                    }
                }
            }
        }
    } else {
        // Standard CHIP-8 8xN Sprite Mode
        for (int row = 0; row < height; ++row) {
            Byte sprite_byte = ram.load(i + row);

            for (int col = 0; col < 8; ++col) {
                if ((sprite_byte & (0x80 >> col)) != 0) {
                    Byte target_x = x_pos + col;
                    Byte target_y = y_pos + row;

                    // Execute SCHIP display clipping quirk
                    if (quirks.clip_sprites_at_edges) {
                        if (target_x >= max_x || target_y >= max_y) {
                            continue;
                        }
                    }

                    // set collision flag if collision was detected
                    if (display.setPixel(target_x, target_y)) {
                        register_table[0xF] = 1;
                    }
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
    int8_t key = keypad.getPressedKey(); 
    if (key == -1) { pc -= 2; } 
    else { register_table[ops.x] = key; }
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

// Point I to the high-res 10-byte font sprite for digit VX
void CPU::I_FX30(const Operands& ops)
{
    i = SCHIP_FONTSET_START_ADDRESS + (register_table[ops.x] * 10);
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

    // legacy quirk
    if (!quirks.load_store_leaves_i_unchanged) {
        i += (ops.x + 1);
    }
}

// load registers V0 through VX from memory starting at I
void
CPU::I_FX65(const Operands& ops)
{

    for (Register index = 0; index <= ops.x; ++index) {
        register_table[index] = ram.load(i + index);
    }

    // legacy quirk
    if (!quirks.load_store_leaves_i_unchanged) {
        i += (ops.x + 1);
    }
}

// Save registers V0 through VX to HP-48 RPL user flags
void CPU::I_FX75(const Operands& ops)
{
    // prevent overflow of 8-byte RPL array if X > 7
    uint8_t limit = (ops.x > 7) ? 7 : ops.x;
    
    for (uint8_t idx = 0; idx <= limit; ++idx) {
        rpl_flags[idx] = register_table[idx];
    }
}

// Load registers V0 through VX from HP-48 RPL user flags
void CPU::I_FX85(const Operands& ops)
{
    // prevent overflow of 8-byte RPL array if X > 7
    uint8_t limit = (ops.x > 7) ? 7 : ops.x;
    
    for (uint8_t idx = 0; idx <= limit; ++idx) {
        register_table[idx] = rpl_flags[idx];
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
