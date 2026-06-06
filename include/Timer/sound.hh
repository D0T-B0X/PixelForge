#ifndef _TIMER_SOUND_HH_
#define _TIMER_SOUND_HH_

#include "Timer/base.hh"

namespace chip8
{

namespace timer
{

class Sound : public Base
{
  private:
    Timer sound;

  public:
    Sound();

    // decrement the value of the timer by 1
    void decrementTimer() override;

    // set the timer to a valid value 
    void setTimer(Timer value) override;

    // Timer read for sound timer is not allowed
    Timer getTimer() override;

};

} // namespace timer
} // namespace chip8

#endif // _TIMER_SOUND_HH_
