#ifndef DRIVER_CONTAINER_H
#define DRIVER_CONTAINER_H

#include "../clockFunctions/drivers/DelayDriver.h"
#include "../controlLEDs/drivers/LEDDriver.h"

class DriverContainer
{

public:
    DriverContainer();

public:
   DelayDriver delayDriver_;
   LEDDriver ledDriver_;
};


#endif