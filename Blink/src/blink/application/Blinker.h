#ifndef BLINKER_H
#define BLINKER_H

#include <stdint.h>

void blink(uint32_t delayTime, int32_t numberOfCycles);
void loopForCycles(uint32_t delayTime, int32_t cycles);
void loopForever(uint32_t delayTime);
void executeLoop(uint32_t delayTime);

#endif