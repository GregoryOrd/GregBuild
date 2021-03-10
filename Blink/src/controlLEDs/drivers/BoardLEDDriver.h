#ifndef BOARD_LED_DRIVER_H
#define BOARD_LED_DRIVER_H

#include <stdint.h>
#include "I_BoardLEDDriver.h"

class BoardLEDDriver : public I_BoardLEDDriver
{
public:
    explicit BoardLEDDriver();

    void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits);
    void writeToPortB(uint8_t portBBits);

    uint8_t read_DDRB();
    uint8_t read_PORTB();
};

#endif