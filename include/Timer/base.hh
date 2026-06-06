/**
 * @file base.hh
 * 
 * All timers must inherit from this base class
 * and implement its functions  
 * 
 */

#ifndef _TIMER_BASE_HH_
#define _TIMER_BASE_HH_

#include "types.hh"

namespace chip8
{

namespace timer
{

class Base
{
  public:
    /**
     * Default destructor
     */
    virtual ~Base() = default;

    /**
     * decrement the counter value with safeguards
     */
    virtual void decrementTimer() { };

    /**
     * set the counter value within the permitted range
     */
    virtual void setTimer(Timer value) { };

    /**
     * return the current value of the timer
     */
    virtual Timer getTimer() { return 0; };
};

} // namespace timer

} // namespace chip8

#endif // _TIMER_BASE_HH_