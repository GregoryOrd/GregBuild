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

void testChars()
{
   char a = 'a';
   char b = 'a';

   G_ASSERT_CHAR_EQ(a, b);
}

void testUnsignedChar()
{
   unsigned char a = 123;
   unsigned char b = 123;

   G_ASSERT_UNSIGNED_CHAR_EQ(a, b);
}

void testSignedChar()
{
   signed char a = 123;
   signed char b = 123;

   G_ASSERT_SIGNED_CHAR_EQ(a, b);
}