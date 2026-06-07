#include <chrono>

#include "system.hh"

namespace chip8
{

System::System()
 : cpu(
    this->ram, 
    this->display, 
    this->delay, 
    this->sound,
    this->keypad,
    this->quirks
   ),
   romLoaded(false)
{
    // seed the RNG for instruction I_CXNN
    std::srand(std::time(nullptr));
    windowManager.initWindow();
}

void
System::updateKeypadInput()
{
    /**
     * Keyboard to keypad mapping:
     *  
     * | 1 | 2 | 3 | 4 | -> | 1 | 2 | 3 | C |
     * |---|---|---|---| -> |---|---|---|---|
     * | Q | W | E | R | -> | 4 | 5 | 6 | D |
     * | A | S | D | F | -> | 7 | 8 | 9 | E |
     * | Z | X | C | V | -> | A | 0 | B | F |
     */
    keypad.updateKeymap(0x1, windowManager.isKeyPressed(GLFW_KEY_1));
    keypad.updateKeymap(0x2, windowManager.isKeyPressed(GLFW_KEY_2));
    keypad.updateKeymap(0x3, windowManager.isKeyPressed(GLFW_KEY_3));
    keypad.updateKeymap(0xC, windowManager.isKeyPressed(GLFW_KEY_4));

    keypad.updateKeymap(0x4, windowManager.isKeyPressed(GLFW_KEY_Q));
    keypad.updateKeymap(0x5, windowManager.isKeyPressed(GLFW_KEY_W));
    keypad.updateKeymap(0x6, windowManager.isKeyPressed(GLFW_KEY_E));
    keypad.updateKeymap(0xD, windowManager.isKeyPressed(GLFW_KEY_R));

    keypad.updateKeymap(0x7, windowManager.isKeyPressed(GLFW_KEY_A));
    keypad.updateKeymap(0x8, windowManager.isKeyPressed(GLFW_KEY_S));
    keypad.updateKeymap(0x9, windowManager.isKeyPressed(GLFW_KEY_D));
    keypad.updateKeymap(0xE, windowManager.isKeyPressed(GLFW_KEY_F));

    keypad.updateKeymap(0xA, windowManager.isKeyPressed(GLFW_KEY_Z));
    keypad.updateKeymap(0x0, windowManager.isKeyPressed(GLFW_KEY_X));
    keypad.updateKeymap(0xB, windowManager.isKeyPressed(GLFW_KEY_C));
    keypad.updateKeymap(0xF, windowManager.isKeyPressed(GLFW_KEY_V));
}

void
System::run()
{
    auto lastTimerTick = std::chrono::steady_clock::now();

    while(!windowManager.shouldWindowClose()) {
        glfwPollEvents();

        if (windowManager.pendingROM) { loadRom(); }

        if (romLoaded) {

            windowManager.processInput();
            updateKeypadInput();

            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTimerTick);
            while (elapsed.count() >= 16) {
                delay.decrementTimer();
                sound.decrementTimer();
                lastTimerTick += std::chrono::milliseconds(16);
                elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTimerTick);
            }

            for(int i = 0; i < 10; ++i)
            {
                cpu.cycle();
            }
        }

        windowManager.render(this->display.getDisplayState());

        windowManager.swapWindowBuffers();
    }

    windowManager.cleanup();
}

void
System::loadRom()
{
    const char* rom_path = windowManager.getROMPath();
    const std::vector<Byte>& binary = romLoader.parseROM(rom_path);
    ram.loadRom(binary);

    windowManager.pendingROM = false;
    romLoaded = true;
}

}