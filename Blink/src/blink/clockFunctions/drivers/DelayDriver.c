#include "DelayDriver.h"

#include <avr/delay.h>

#define DELAY_DRIVER_C

void delayMS(uint32_t msToDelay) { _delay_ms(msToDelay); }