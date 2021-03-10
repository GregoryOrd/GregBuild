#include <stdio.h>

#include "../../GregTest.h"
#include "../DetermineDelays.h"

void willReturnCorrectBlinkDelay()
{
   uint8_t actual = blinkingDelay();

   if (actual == 1000)
   {
      printf("willWriteToDataDirectionRegisterCorrectlty Passed\n");
   }
   else
   {
      printf("willWriteToDataDirectionRegisterCorrectlty Failed\n");
   }
}