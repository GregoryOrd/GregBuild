#ifndef I_BOARD_LED_DRIVER_H
#define I_BOARD_LED_DRIVER_H

#include <stdint.h>

class I_BoardLEDDriver
{
public:
    void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits){};
    void writeToPortB(uint8_t portBBits){};

    uint8_t read_DDRB(){};
    uint8_t read_PORTB(){};
};

#endif