#include "Timer/sound.hh"

namespace chip8
{

namespace timer
{

Sound::Sound()
 : sound(0)
{
}

// decrement the sound timer by 1
void 
Sound::decrementTimer()
{
    if (sound > 0) sound--;
}

// set the sound timer's value
void 
Sound::setTimer(Timer value)
{
    sound = value;
}

// This function does not exist for the sound timer
// but is implemented to satisfy the base class requirements
Timer
Sound::getTimer()
{
    return 0;
}

}
}