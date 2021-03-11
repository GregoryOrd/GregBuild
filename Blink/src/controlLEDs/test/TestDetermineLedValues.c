#include <stdint.h>
#include <stdio.h>

#include "../../GregTest.h"
#include "../application/DetermineLedValues.h"

void willReturnCorrectBitsForDataDirectionRegister()
{
   uint8_t actual = bitsForDataDirectionRegister();

   if (actual == 0b00100001)
   {
      printf("willWriteToDataDirectionRegisterCorrectlty Passed\n");
   }
   else
   {
      printf("willWriteToDataDirectionRegisterCorrectlty Failed\n");
   }
}
