#ifndef _TIMER_DELAY_HH_
#define _TIMER_DELAY_HH_

#include "Timer/base.hh"

namespace chip8
{

namespace timer
{

class Delay : public Base
{
  private:
    Timer delay;

  public:
    Delay();

    // decrement the timer by 1
    void decrementTimer() override;

    // set the timer to a valid value
    void setTimer(Timer value) override;

    // return the current timer value
    Timer getTimer() override;

};

} // namespace timer
} // namespace chip8

#endif // _TIMER_DELAY_HH_
