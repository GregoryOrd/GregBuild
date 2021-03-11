/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */

#include "../clockFunctions/Delays.h"
#include "../controlLEDs/LEDController.h"
#include "../controlLEDs/LEDDriver.h"

int main(void)
{
   LEDDriver driver = LEDDriver();
   LEDController ledController = LEDController(driver);
   ledController.configureBoardLEDForWriting();
   while (1)
   {
      ledController.turnOnBoardLED();
      delayForBlink();
      ledController.turnOffBoardLED();
      delayForBlink();
   }
}
