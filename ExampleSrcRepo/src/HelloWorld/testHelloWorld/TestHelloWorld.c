#include <GregTest.h>
#include <stdio.h>

#include "../HelloWorld.h"

void testMySt456uff()
{
   const char* expected = "Hello World";
   const char* actual = "Hello World";

   G_ASSERT_STR_EQ(expected, actual);
}

void testMySt456f()
{
   const char* expected = "Hello World";
   const char* actual = "Hello World";

   G_ASSERT_STR_EQ(expected, actual);
}

void testIntegers()
{
   int a = 5;
   int b = 5;

   G_ASSERT_INT_EQ(a, b);
}