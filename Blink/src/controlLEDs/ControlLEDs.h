#ifndef CONTROL_LEDS_H
#define CONTROL_LEDS_H

#include <stdint.h>

class ControlLEDs
{
public:
    ControlLEDs();

public:
   void configureBoardLEDForWriting();
   void turnOnBoardLED();
   void turnOffBoardLED();
   void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits);
   void writeToPortB(uint8_t portBBits);
};

#endif
