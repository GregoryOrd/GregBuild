#include "WritePortB.h"

#include <avr/io.h>

void writeToDataDirectionRegisterB() { DDRB = 0b00100001; }

void writeToPortBToTurnLEDsOn() { PORTB = 0b00100001; }

void writeToPortBToTurnLEDsOff() { PORTB = 0b00000000; }