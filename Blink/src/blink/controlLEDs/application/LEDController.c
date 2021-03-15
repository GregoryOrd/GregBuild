#include "LEDController.h"

#include "../drivers/LEDDriver.h"

void configureBoardLEDForWriting() 
{
    writeToDataDirectionRegisterB(determineBitPatternForConfiguringLEDsForWriting()); 
}

void turnOnBoardLED() 
{ 
    writeToPortB(determineBitPatternForTurningOnTheBoardLED()); 
}

void turnOffBoardLED() 
{
    writeToPortB(determineBitPatternForTurningOffTheBoardLED()); 
}

uint8_t determineBitPatternForConfiguringLEDsForWriting()
{
    return 0b00100001;
}

uint8_t determineBitPatternForTurningOnTheBoardLED()
{
    return 0b00100001;
}

uint8_t determineBitPatternForTurningOffTheBoardLED()
{
    return 0b00000000;
}