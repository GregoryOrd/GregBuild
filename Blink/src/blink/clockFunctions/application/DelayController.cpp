#include "DelayController.h"

#define __DELAY_BACKWARD_COMPATIBLE__

DelayController::DelayController(DelayDriver driver) : driver_(driver)
{
}

void DelayController::delayForBlink()
{
    driver_.delayMS(100);
}
