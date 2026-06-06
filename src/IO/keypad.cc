#include "IO/keypad.hh"

namespace chip8
{

namespace io
{

Keypad::Keypad()
{
    for(int i = 0; i < 16; ++i) { keymap[i] = false; }
}

void
Keypad::updateKeymap(Byte key, bool state)
{
    if (key > 0xF) {
        EPRINTF("FATAL: Invalid key map recieved.\n")
        std::exit(-1);
    }

    keymap[key] = state;
}

bool
Keypad::isKeyPressed(Byte key)
{
    if (key > 0xF) {
        EPRINTF("FATAL: Invalid key map recieved.\n")
        std::exit(-1);
    }

    return keymap[key];
}

int8_t
Keypad::getPressedKey() const
{
    // return the key index if any valid pressed key is found
    for(int i = 0; i < 16; ++i) { if (keymap[i]) return i; }

    return -1;
}

} // namespace io
} // namespace chip8