#include "LEDDriver.h"

#include <avr/io.h>

LEDDriver::LEDDriver()
{
}

void LEDDriver::writeToDataDirectionRegisterB(uint8_t dataDirectionBBits)
{ 
    DDRB = dataDirectionBBits; 
}


void LEDDriver::writeToPortB(uint8_t portBBits)
{ 
    PORTB = portBBits; 
}