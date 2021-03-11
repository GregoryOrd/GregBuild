#include "DelayController.h"
#include "DetermineDelays.h"

#define __DELAY_BACKWARD_COMPATIBLE__

DelayController::DelayController(DelayDriver driver) : driver_(driver)
{
}

void DelayController::delayForBlink()
{
    driver_.delayMS(blinkingDelay());
}
