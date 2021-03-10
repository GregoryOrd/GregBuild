#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/delay.h>
#include <stdint.h>

#include "Delays.h"
#include "DetermineDelays.h"

void delayForBlink() { _delay_ms(blinkingDelay()); }