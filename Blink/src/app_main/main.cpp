/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */

#include "../clockFunctions/application/DelayController.h"
#include "../controlLEDs/application/LEDController.h"

int main(void)
{
   DelayDriver delayDriver = DelayDriver();
   DelayController delayController = DelayController(delayDriver);

   LEDDriver ledDriver = LEDDriver();
   LEDController ledController = LEDController(ledDriver);
   
   ledController.configureBoardLEDForWriting();
   while (1)
   {
      ledController.turnOnBoardLED();
      delayController.delayForBlink();
      ledController.turnOffBoardLED();
      delayController.delayForBlink();
   }
}
