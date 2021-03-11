#include "LEDController.h"

#include <avr/io.h>

#include "DetermineLedValues.h"
#include "LEDDriver.h"

LEDController::LEDController(LEDDriver driver) : driver_(driver)
{
}

void LEDController::configureBoardLEDForWriting() { driver_.writeToDataDirectionRegisterB(bitsForDataDirectionRegister()); }

void LEDController::turnOnBoardLED() { driver_.writeToPortB(bitsForTurningOnLed()); }

void LEDController::turnOffBoardLED() { driver_.writeToPortB(bitsForTurningOffLed()); }