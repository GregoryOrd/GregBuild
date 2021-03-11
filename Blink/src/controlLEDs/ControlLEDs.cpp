#include "ControlLEDs.h"

#include <avr/io.h>

#include "DetermineLedValues.h"

ControlLEDs::ControlLEDs(/* args */)
{
}

void ControlLEDs::configureBoardLEDForWriting() { writeToDataDirectionRegisterB(bitsForDataDirectionRegister()); }

void ControlLEDs::turnOnBoardLED() { writeToPortB(bitsForTurningOnLed()); }

void ControlLEDs::turnOffBoardLED() { writeToPortB(bitsForTurningOffLed()); }

void ControlLEDs::writeToDataDirectionRegisterB(uint8_t dataDirectionBBits) { DDRB = dataDirectionBBits; }
void ControlLEDs::writeToPortB(uint8_t portBBits) { PORTB = portBBits; }