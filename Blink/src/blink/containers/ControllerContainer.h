#ifndef CONTROLLER_CONTAINER_H
#define CONTROLLER_CONTAINER_H

#include "../clockFunctions/application/DelayController.h"
#include "../controlLEDs/application/LEDController.h"
#include "DriverContainer.h"

class ControllerContainer
{
public:
    ControllerContainer(DriverContainer drivers);

public:
   DelayController delayController_;
   LEDController ledController_;
};


#endif