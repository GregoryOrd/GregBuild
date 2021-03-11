#ifndef DELAY_CONTROLLER_H
#define DELAY_CONTROLLER_H

#include "../drivers/DelayDriver.h"

class DelayController
{

public:
    DelayController(DelayDriver driver);

public:
    void delayForBlink();

private:
    DelayDriver driver_;
};

#endif