#include "LEDDriver.h"

#include <avr/io.h>

#define LED_DRIVER_C

void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits)
{
   // Eventually, we can task this out to a toolkit level c or asm function
   DDRB = dataDirectionBBits;
}

void writeToPortB(uint8_t portBBits)
{
   // Eventually, we can task this out to a toolkit level c or asm function
   PORTB = portBBits;
}