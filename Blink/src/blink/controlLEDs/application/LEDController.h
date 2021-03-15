#ifndef CONTROL_LEDS_H
#define CONTROL_LEDS_H

#include <stdint.h>

void configureBoardLEDForWriting();
void turnOnBoardLED();
void turnOffBoardLED();

uint8_t determineBitPatternForConfiguringLEDsForWriting();
uint8_t determineBitPatternForTurningOnTheBoardLED();
uint8_t determineBitPatternForTurningOffTheBoardLED();


#endif
