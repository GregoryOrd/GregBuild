#include <stdint.h>
#include <stdio.h>

#include "../../GregTest.h"
#include "../application/ControlLEDs.h"
#include "../application/DetermineLedValues.h"
#include "../drivers/StubBoardLEDDriver.h"

void willConfigureBoardLEDForWriting()
{
   uint8_t DDR_bits = bitsForDataDirectionRegister();
   StubBoardLEDDriver* ledDriver = new StubBoardLEDDriver();
   ControlLEDs* ledController = new ControlLEDs(*ledDriver);

   ledController->configureBoardLEDForWriting();

   //VERIFY ledDriver->read_DDRB() == 0b00100001;

   delete ledController;
   delete ledDriver;
}

void willTurnOnBoardLED()
{
   uint8_t ledOnBits = bitsForTurningOnLed();
   StubBoardLEDDriver* ledDriver = new StubBoardLEDDriver();
   ControlLEDs* ledController = new ControlLEDs(*ledDriver);

   ledController->turnOnBoardLED();

   //VERIFY ledDriver->read_PORTB() == 0b00100001;

   delete ledController;
   delete ledDriver;
}

void willTurnOffBoardLED()
{
   uint8_t ledOffBits = bitsForTurningOffLed();
   StubBoardLEDDriver* ledDriver = new StubBoardLEDDriver();
   ControlLEDs* ledController = new ControlLEDs(*ledDriver);

   ledController->turnOffBoardLED();

   //VERIFY ledDriver->read_PORTB() == 0b00000000;

   delete ledController;
   delete ledDriver;
}