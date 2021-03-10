/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */

#include "clockFunctions/Delays.h"
#include "controlLEDs/application/ControlLEDs.h"
#include "controlLEDs/drivers/BoardLEDDriver.h"
#include "controlLEDs/drivers/I_BoardLEDDriver.h"

int main(void)
{
   BoardLEDDriver ledDriver = BoardLEDDriver();
   ControlLEDs ledController = ControlLEDs(ledDriver);

   ledController.configureBoardLEDForWriting();
   while (1)
   {
      ledController.turnOnBoardLED();
      delayForBlink();
      ledController.turnOffBoardLED();
      delayForBlink();
   }
}
