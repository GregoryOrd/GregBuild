#ifndef CONTROL_LEDS_H
#define CONTROL_LEDS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void configureBoardLEDForWriting();
void turnOnBoardLED();
void turnOffBoardLED();
void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits);
void writeToPortB(uint8_t portBBits);

#ifdef __cplusplus
}
#endif

#endif
