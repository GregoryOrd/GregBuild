#include "Blinker.h"

#include "../controlLEDs/application/LEDController.h"
#include "../clockFunctions/application/DelayController.h"


void blink(uint32_t delayTime, int32_t numberOfCycles)
{
   configureBoardLEDForWriting();
   if(numberOfCycles > 0)
   {
      loopForCycles(delayTime, numberOfCycles);
   }
   else
   {
      loopForever(delayTime);
   }

}

void loopForCycles(uint32_t delayTime, int32_t cycles)
{
   int32_t i = 0;
   while(i < cycles)
   {
      executeLoop(delayTime);
      i++;
   }
}

void loopForever(uint32_t delayTime)
{
   while (1)
   {
      executeLoop(delayTime);
   }
}

void executeLoop(uint32_t delayTime)
{
   turnOnBoardLED();
   delayForBlink(delayTime);
   turnOffBoardLED();
   delayForBlink(delayTime);
}