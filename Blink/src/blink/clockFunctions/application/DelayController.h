#ifndef DELAY_CONTROLLER_H
#define DELAY_CONTROLLER_H

#include "../drivers/DelayDriver.h"
#include <stdint.h>

class DelayController
{

public:
    DelayController(DelayDriver driver);

public:
    void delayForBlink(uint32_t delayTime);

private:
    DelayDriver driver_;
};

#endif