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

void testShort()
{
   short a = 123;
   short b = 123;

   G_ASSERT_INT_EQ(a, b);
}

void testLong()
{
   long a = 3147483649;
   long b = 3147483649;

   G_ASSERT_LONG_EQ(a, b);
}

void testUnsignedInt()
{
   unsigned int a = 4294967291;
   unsigned int b = 4294967291;

   G_ASSERT_UNSIGNED_INT_EQ(a, b);
}

void testUnsignedLong()
{
   unsigned long a = 18446744073709551611u;
   unsigned long b = 18446744073709551611u;

   G_ASSERT_UNSIGNED_LONG_EQ(a, b);
}

void testFloat()
{
   double a = 123.26;
   double b = 123.26;

   G_ASSERT_FLOAT_EQ(a, b);
}

void testDouble()
{
   double a = 123.26;
   double b = 123.26;

   G_ASSERT_DOUBLE_EQ(a, b);
}

void testLongDouble()
{
   double a = 123.2500121236;
   double b = 123.2500121236;

   G_ASSERT_DOUBLE_EQ(a, b);
}

void testFloatNear()
{
   float a = 123.25;
   float b = 123.75;
   float epsilon = 1.5;

   G_ASSERT_FLOAT_NEAR(a, b, epsilon);
}

void testDoubleNear()
{
   double a = 123.0000025;
   double b = 123.0000075;
   double epsilon = 1.000005;

   G_ASSERT_DOUBLE_NEAR(a, b, epsilon);
}