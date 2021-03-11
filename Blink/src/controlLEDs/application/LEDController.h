#ifndef CONTROL_LEDS_H
#define CONTROL_LEDS_H

#include "../drivers/LEDDriver.h"

class LEDController
{
public:
    LEDController(LEDDriver driver);

public:
   void configureBoardLEDForWriting();
   void turnOnBoardLED();
   void turnOffBoardLED();

private:
    LEDDriver driver_;
};

#endif
