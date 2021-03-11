/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */

#include "../blink/clockFunctions/application/DelayController.h"
#include "../blink/controlLEDs/application/LEDController.h"
#include "../blink/containers/DriverContainer.h"

int main(void)
{
   DriverContainer driverContainer = DriverContainer();
   DelayDriver delayDriver = driverContainer.delayDriver_;
   LEDDriver ledDriver = driverContainer.ledDriver_;

   DelayController delayController = DelayController(delayDriver);
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
