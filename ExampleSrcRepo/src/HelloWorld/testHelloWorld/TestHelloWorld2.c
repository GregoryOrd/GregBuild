#include <stdio.h>

#include "../../GregTest.h"
#include "../HelloWorld.h"

void testHelloWorldString(){
   const char* expected = "Hello World";
   const char* actual = "Hello World";

   G_ASSERT_STR_EQ(expected, actual);
}

void testM(){
    const char* expected = "Hello World";
    const char* actual = "Hello World";

    G_ASSERT_STR_EQ(expected, actual);
}
