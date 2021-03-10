#include "ControlLEDs.h"

#include <avr/io.h>

#include "DetermineLedValues.h"

void configureBoardLEDForWriting() { writeToDataDirectionRegisterB(bitsForDataDirectionRegister()); }

void turnOnBoardLED() { writeToPortB(bitsForTurningOnLed()); }

void turnOffBoardLED() { writeToPortB(bitsForTurningOffLed()); }

void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits) { DDRB = dataDirectionBBits; }
void writeToPortB(uint8_t portBBits) { PORTB = portBBits; }