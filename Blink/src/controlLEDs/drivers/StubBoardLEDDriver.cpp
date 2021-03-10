#include "StubBoardLEDDriver.h"

StubBoardLEDDriver::StubBoardLEDDriver() {}

void StubBoardLEDDriver::writeToDataDirectionRegisterB(uint8_t dataDirectionBBits) { DDRB_ = dataDirectionBBits;}

void StubBoardLEDDriver::writeToPortB(uint8_t portBBits) { PORTB_ = portBBits;}

uint8_t StubBoardLEDDriver::read_DDRB()
{
   return DDRB_;
}

uint8_t StubBoardLEDDriver::read_PORTB()
{
   return PORTB_;
}