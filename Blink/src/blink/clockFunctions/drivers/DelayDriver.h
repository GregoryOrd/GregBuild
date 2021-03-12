#ifndef DELAY_DRIVER_H
#define DELAY_DRIVER_H

#include <stdint.h>

class DelayDriver
{

public:
    DelayDriver();

public:
    void delayMS(uint32_t msToDelay);
};


#endif