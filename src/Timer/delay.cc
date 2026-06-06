#include "Timer/delay.hh"

namespace chip8
{

namespace timer
{

Delay::Delay()
 : delay(0)
{
}

// decrement the Delay timer by 1
void 
Delay::decrementTimer()
{
    if (delay > 0) delay--;
}

// set the Delay timer's value
void 
Delay::setTimer(Timer value)
{
    delay = value;
}

// returns the current value of
// the delay timer
Timer
Delay::getTimer()
{
    return delay;
}

}
}