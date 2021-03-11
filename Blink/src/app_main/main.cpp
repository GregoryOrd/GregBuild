/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */

#include "../blink/containers/ControllerContainer.h"
#include "../blink/containers/DriverContainer.h"
#include "../blink/application/Blinker.h"

int main(void)
{
   DriverContainer driverContainer = DriverContainer();
   ControllerContainer controllerContainer = ControllerContainer(driverContainer);

   Blinker blinker = Blinker(controllerContainer);
   blinker.blink(100, 0x000000a);
}
