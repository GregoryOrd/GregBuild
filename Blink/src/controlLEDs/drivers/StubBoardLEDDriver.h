#ifndef STUB_BOARD_LED_DRIVER_H
#define STUB_BOARD_LED_DRIVER_H

#include <stdint.h>
#include "I_BoardLEDDriver.h"

class StubBoardLEDDriver : public I_BoardLEDDriver
{
public:
    explicit StubBoardLEDDriver();

    void writeToDataDirectionRegisterB(uint8_t dataDirectionBBits);
    void writeToPortB(uint8_t portBBits);

    uint8_t read_DDRB();
    uint8_t read_PORTB();

private:
   uint8_t DDRB_;
   uint8_t PORTB_;
};

#endif