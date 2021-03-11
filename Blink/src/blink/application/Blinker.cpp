#include "Blinker.h"

Blinker::Blinker(ControllerContainer controllers)
   : delayController_(controllers.delayController_)
   , ledController_(controllers.ledController_)
{
}

void Blinker::blink(uint32_t delayTime, int32_t numberOfCycles)
{
   ledController_.configureBoardLEDForWriting();
   if(numberOfCycles > 0)
   {
      loopForCycles(delayTime, numberOfCycles);
   }
   else
   {
      loopForever(delayTime);
   }

}

void Blinker::loopForCycles(uint32_t delayTime, int32_t cycles)
{
   int32_t i = 0;
   while(i < cycles)
   {
      executeLoop(delayTime);
      i++;
   }
}

void Blinker::loopForever(uint32_t delayTime)
{
   while (1)
   {
      executeLoop(delayTime);
   }
}

void Blinker::executeLoop(uint32_t delayTime)
{
   ledController_.turnOnBoardLED();
   delayController_.delayForBlink(delayTime);
   ledController_.turnOffBoardLED();
   delayController_.delayForBlink(delayTime);
}