#ifndef BLINKER_H
#define BLINKER_H

#include "../containers/ControllerContainer.h"

class Blinker
{

public:
    Blinker(ControllerContainer controllers);

public:
   void blink(uint32_t delayTime, int32_t numberOfCycles);

private:
   void loopForCycles(uint32_t delayTime, int32_t cycles);
   void loopForever(uint32_t delayTime);
   void executeLoop(uint32_t delayTime);

private:
   DelayController delayController_;
   LEDController ledController_;
};


#endif