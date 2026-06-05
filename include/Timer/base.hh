/**
 * @file base.hh
 * 
 * All timers must inherit from this base class
 * and implement its functions  
 * 
 */

#ifndef _TIMER_BASE_HH_
#define _TIMER_BASE_HH_

namespace chip8
{

namespace timer
{

class Base
{
  public:
    /**
     * decrement the counter value with safeguards
     */
    virtual void decrementTimer() { };

    /**
     * set the counter value within the permitted range
     */
    virtual void setCounterTime() { };
};

} // namespace timer

} // namespace chip8

#endif // _TIMER_BASE_HH_