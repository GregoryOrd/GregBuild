#include "DetermineLedValues.h"

uint8_t bitsForDataDirectionRegister() { return 0b00100001; }

uint8_t bitsForTurningOnLed() { return 0b00100001; }

uint8_t bitsForTurningOffLed() { return 0b00000000; }