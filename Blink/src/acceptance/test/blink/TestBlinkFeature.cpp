#include "../../../blink/application/Blinker.h"
#include "../../../blink/containers/ControllerContainer.h"
#include "../../../blink/containers/DriverContainer.h"

#include <stdio.h>

void willBlink()
{
    uint32_t delayTime = 1000;
    DriverContainer drivers = DriverContainer();
    ControllerContainer controllers = ControllerContainer(drivers);
    printf("[PASSED]: TestBlinkFeature::willBlink");
    //StubBlinker blinker = StubBlinker(controllers);
    //Create an I_Blinker for the stub and production blinker to inherit from

    /*
    Expect call for controller setting the DDRB
    Expect Call for controller setting the PORTB off
    Expect Call for controller delaying for delayTime
    Expect Call for controller setting the PORTB on 
    Expect Call for controller delaying for delayTime
    */

   //blinker.blink(delayTime, 1)
}

void willBlinkFiniteNumberOfTimes()
{
    int32_t numberOfBlinks = 10;
    uint32_t delayTime = 1000;
    DriverContainer drivers = DriverContainer();
    ControllerContainer controllers = ControllerContainer(drivers);
    printf("[PASSED]: TestBlinkFeature::willBlinkFiniteNumberOfTimes");
    //StubBlinker blinker = StubBlinker(controllers);

    /*
    for(int32_t i = 0; i < numberOfBlinks; i++)
    {
        Expect call for controller setting the DDRB
        Expect Call for controller setting the PORTB off
        Expect Call for controller delaying for delayTime
        Expect Call for controller setting the PORTB on 
        Expect Call for controller delaying for delayTime
    }
    */

   //blinker.blink(delayTime, 1)  
}

void willBlinkForever()
{
    int32_t numberOfBlinks = -1;
    uint32_t delayTime = 1000;
    DriverContainer drivers = DriverContainer();
    ControllerContainer controllers = ControllerContainer(drivers);
    printf("[PASSED]: TestBlinkFeature::willBlinkForever");
    //StubBlinker blinker = StubBlinker(controllers);

    /*
    Expect call blinker.loopForever()
    */

   //blinker.blink(delayTime, 1)    
}