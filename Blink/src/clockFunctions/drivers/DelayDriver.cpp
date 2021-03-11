#include "DelayDriver.h"
#include <avr/delay.h>

DelayDriver::DelayDriver()
{
}

void DelayDriver::delayMS(uint32_t msToDelay)
{
    _delay_ms(msToDelay);
}