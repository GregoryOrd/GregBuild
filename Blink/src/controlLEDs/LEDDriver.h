#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

class LEDDriver
{

public:
    LEDDriver();

public:
    void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits);
    void writeToPortB(uint8_t portBBits);
};


#endif