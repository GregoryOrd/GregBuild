#include "HelloWorld.h"

#include <stdio.h>

#include "Hello.h"
#include "World.h"

void main() { printHelloWorld(); }

void printHelloWorld()
{
   char helloWorldStr[255];
   helloWorldString(helloWorldStr);
   printf("%s\n", helloWorldStr);
}

void helloWorldString(char* helloWorldStr)
{
   hello(helloWorldStr);
   world(helloWorldStr);
}
