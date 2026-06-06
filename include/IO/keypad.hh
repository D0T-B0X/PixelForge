#ifndef _IO_KEYPAD_HH_
#define _IO_KEYPAD_HH_

#include "types.hh"

namespace chip8
{

namespace io
{

class Keypad
{
  private:
    bool keymap[16];

  public:
    Keypad();

    /**
     * @brief Update the keymap with key states
     * 
     * @param key The key being referenced
     * @param state The state of the key (pressed or not)
     */
    void updateKeymap(Byte key, bool state);

    /**
     * @brief Poll the state of a specified key
     * 
     * @param key The key being polled
     * @return True if the key is pressed.
     * @return False if the key is not pressed
     */
    bool isKeyPressed(Byte key);

    /**
     * @brief Returns the index of the pressed key
     * 
     * @return pressed key index if found, -1 otherwise
     */
    int8_t getPressedKey() const;
};

} // namespace io
} // namespace chip8

#endif // _IO_KEYPAD_HH_
