#include "Blinker.h"

Blinker::Blinker(ControllerContainer controllers)
   : delayController_(controllers.delayController_)
   , ledController_(controllers.ledController_)
{
}

void Blinker::blink(uint32_t delayTime)
{
   ledController_.configureBoardLEDForWriting();
   while (1)
   {
      ledController_.turnOnBoardLED();
      delayController_.delayForBlink(delayTime);
      ledController_.turnOffBoardLED();
      delayController_.delayForBlink(delayTime);
   }
}