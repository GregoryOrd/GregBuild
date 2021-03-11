/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */

#include "../clockFunctions/Delays.h"
#include "../controlLEDs/ControlLEDs.h"

int main(void)
{
   ControlLEDs ledController = ControlLEDs();
   ledController.configureBoardLEDForWriting();
   while (1)
   {
      ledController.turnOnBoardLED();
      delayForBlink();
      ledController.turnOffBoardLED();
      delayForBlink();
   }
}
