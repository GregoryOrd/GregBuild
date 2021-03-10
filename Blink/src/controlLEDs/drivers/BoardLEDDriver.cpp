#include "BoardLEDDriver.h"

#include <avr/io.h>

BoardLEDDriver::BoardLEDDriver() {}

void BoardLEDDriver::writeToDataDirectionRegisterB(uint8_t dataDirectionBBits) 
{ 
   //Eventually call this from an avr toolkit function
   DDRB = dataDirectionBBits; 
}

void BoardLEDDriver::writeToPortB(uint8_t portBBits) 
{ 
   //Eventually call this from an avr toolkit function
   PORTB = portBBits; 
}

uint8_t BoardLEDDriver::read_DDRB()
{
   //Eventually call this from an avr toolkit function
   return DDRB;
}

uint8_t BoardLEDDriver::read_PORTB()
{
   //Eventually call this from an avr toolkit function
   return PORTB;
}