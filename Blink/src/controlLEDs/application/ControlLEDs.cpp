#include "ControlLEDs.h"

#include <avr/io.h>

#include "../drivers/BoardLEDDriver.h"
#include "DetermineLedValues.h"

#include "../drivers/I_BoardLEDDriver.h"

ControlLEDs::ControlLEDs(I_BoardLEDDriver ledDriver)
   : ledDriver_(ledDriver)
{
}

ControlLEDs::~ControlLEDs()
{
}

void ControlLEDs::configureBoardLEDForWriting()
{
    uint8_t ddrb_bits = bitsForDataDirectionRegister();
    ledDriver_.writeToDataDirectionRegisterB(ddrb_bits); 
}

void ControlLEDs::turnOnBoardLED() {  ledDriver_.writeToPortB(bitsForTurningOnLed()); }

void ControlLEDs::turnOffBoardLED() {  ledDriver_.writeToPortB(bitsForTurningOffLed()); }