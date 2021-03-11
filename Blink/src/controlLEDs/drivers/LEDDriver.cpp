#include "LEDDriver.h"

#include <avr/io.h>

LEDDriver::LEDDriver()
{
}

void LEDDriver::writeToDataDirectionRegisterB(uint8_t dataDirectionBBits)
{ 
    //Eventually, we can task this out to a toolkit level c or asm function
    DDRB = dataDirectionBBits; 
}


void LEDDriver::writeToPortB(uint8_t portBBits)
{ 
    //Eventually, we can task this out to a toolkit level c or asm function
    PORTB = portBBits; 
}