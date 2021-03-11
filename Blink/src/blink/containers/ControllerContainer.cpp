#include "ControllerContainer.h"

ControllerContainer::ControllerContainer(DriverContainer drivers)
   : delayController_(drivers.delayDriver_)
   , ledController_(drivers.ledDriver_)
{
}