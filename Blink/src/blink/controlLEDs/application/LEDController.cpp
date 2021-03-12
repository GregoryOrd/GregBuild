#include "LEDController.h"

LEDController::LEDController(LEDDriver driver) : driver_(driver)
{
}

void LEDController::configureBoardLEDForWriting() { driver_.writeToDataDirectionRegisterB(0b00100001); }

void LEDController::turnOnBoardLED() { driver_.writeToPortB(0b00100001); }

void LEDController::turnOffBoardLED() { driver_.writeToPortB(0b00000000); }