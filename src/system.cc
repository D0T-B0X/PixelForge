#include "system.hh"

namespace chip8
{

System::System()
 : cpu(
    this->ram, 
    this->display, 
    this->delay, 
    this->sound,
    this->keypad
   )
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
    while(!windowManager.shouldWindowClose()) {
        windowManager.processInput();
        updateKeypadInput();

        cpu.cycle();

        windowManager.swapWindowBuffers();
        glfwPollEvents();
    }

    windowManager.cleanup();
}

void
System::loadRom(const char* path)
{
    ram.loadRom(romLoader.parseROM(path));
}

}