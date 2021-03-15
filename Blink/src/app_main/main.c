/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */


#include "../blink/clockFunctions/application/DelayController.h"
#include "../blink/application/Blinker.h"

int main(void)
{
   blink(100, 3);
   delayForBlink(4000);
}
