#include "DelayController.h"

DelayController::DelayController(DelayDriver driver) : driver_(driver)
{
}

void DelayController::delayForBlink(uint32_t delayTime)
{
    driver_.delayMS(delayTime);
}
