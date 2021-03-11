#ifndef DELAY_DRIVER_H
#define DELAY_DRIVER_H

#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdint.h>

class DelayDriver
{

public:
    DelayDriver();

public:
    void delayMS(uint32_t msToDelay);
};


#endif