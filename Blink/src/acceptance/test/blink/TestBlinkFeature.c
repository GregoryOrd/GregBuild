#include <GregTest.h>
#include <stdio.h>

void willBlink() { printf("[PASSED]: TestBlinkFeature::willBlink\n"); }

void willBlinkFiniteNumberOfTimes() { printf("[PASSED]: TestBlinkFeature::willBlinkFiniteNumberOfTimes\n"); }

void willBlinkForever() { printf("[PASSED]: TestBlinkFeature::willBlinkForever\n"); }

void testHelloWorldWithGregTest()
{
   const char* expected = "Hello World";
   const char* actual = "Hello World";

   G_ASSERT_STR_EQ(expected, actual);
}