/**
 * @file system.hh
 * 
 * This file is the central timing control manager for
 * all the components of the CHIP8 emulator
 */

#ifndef _SYSTEM_HH_
#define _SYSTEM_HH_

#include "types.hh"
#include "Memory/ram.hh"
#include "Processor/cpu.hh"
#include "Display/video.hh"
#include "Timer/delay.hh"
#include "Timer/sound.hh"
#include "Util/Window/manager.hh"
#include "Util/rom_loader.hh"

namespace chip8
{

class System
{
  private:
    // system RAM (4KB)
    memory::RAM ram;

    // display pixel controller
    display::Video display;

    // the sound and delay hardware timers
    timer::Delay delay;
    timer::Sound sound;

    // the processor of the CHIP8 VM
    processor::CPU cpu;

    // Emulator graphics window manager
    util::window::Manager windowManager;
        
  public:
    System();

    void run();
};

} // namespace chip8

#endif // _SYSTEM_HH_
