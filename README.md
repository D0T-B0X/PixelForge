# PixelForge

## Track

Retro & Emulator

## Problem Statement

Build a working **Chip-8 emulator** that can load and run simple Chip-8 programs or games.

Chip-8 is a small virtual machine used for classic-style games and is one of the most beginner-friendly emulator projects. Your goal is to build the core parts of the emulator and run at least one legal Chip-8 ROM, demo, or self-written test program.

The focus is on building a real emulator for a real retro virtual system, while keeping the scope achievable in 36 hours.

## Theme

Build the machine. Run the game.

## Core Requirements

Your emulator must include:

1. **Memory**
   - 4 KB memory.
   - Program loaded into memory starting at the standard Chip-8 program address.

2. **Registers**
   - 16 general-purpose registers: V0 to VF.
   - Index register I.
   - Program counter.
   - Stack and stack pointer.

3. **Instruction Cycle**
   - Fetch instruction.
   - Decode instruction.
   - Execute instruction.

4. **Display**
   - 64 x 32 monochrome display.
   - Ability to clear the screen.
   - Ability to draw sprites.

5. **Input**
   - Map the Chip-8 keypad to a modern keyboard layout.

6. **Timers**
   - Delay timer.
   - Sound timer, even if sound is represented using a simple beep or visual indicator.

7. **ROM Loading**
   - Load at least one legal Chip-8 ROM, public-domain demo, homebrew game, or self-written test program.

## Minimum Viable Product

A successful MVP should have:

- A Chip-8 emulator window or browser page.
- A ROM loader.
- Working display.
- Working keyboard input.
- Enough implemented instructions to run at least one simple ROM or test program.
- A README explaining how to run the emulator.

## Important Legal Rule

Do not include copyrighted game ROMs.

You may use:

- Public-domain Chip-8 ROMs.
- Homebrew Chip-8 ROMs.
- Self-written test ROMs.
- Simple demo programs created by your team.

## Suggested Tech Stack

You may use any stack, including:

- JavaScript + HTML Canvas
- Python + Pygame
- C++
- Rust
- Java
- Go

A browser-based emulator is recommended for easy demoing.

## Suggested Keyboard Mapping

Original Chip-8 keypad:

| 1 | 2 | 3 | C |
|---|---|---|---|
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

Suggested modern keyboard mapping:

| 1 | 2 | 3 | 4 |
|---|---|---|---|
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |

## Stretch Goals

- Visual debugger
- Register viewer
- Memory viewer
- Current opcode display
- Pause/resume
- Step one instruction at a time
- Speed control
- Sound support
- Drag-and-drop ROM loading
- Multiple demo ROMs

## AI Tool Usage

You may use AI tools for:

- Understanding Chip-8 instructions
- Debugging opcode behavior
- Generating UI code
- Creating test programs
- Writing documentation

You must mention in your final README how AI tools were used.

## Demo Expectations

Your final demo should show:

1. Loading a Chip-8 ROM or test program.
2. Running the program.
3. Display output working.
4. Keyboard input working.
5. A short explanation of the emulator loop.
6. Optional: debugger or register view.

## Judging Focus

You will be judged on:

- Emulator correctness
- Technical implementation
- Ability to run a real Chip-8 program
- Clarity of demo
- Documentation
- Scope management
