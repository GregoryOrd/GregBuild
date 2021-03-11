#ifndef DETERMINE_LED_VALUES
#define DETERMINE_LED_VALUES

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t bitsForDataDirectionRegister();
uint8_t bitsForTurningOnLed();
uint8_t bitsForTurningOffLed();

#ifdef __cplusplus
}
#endif

#endif
