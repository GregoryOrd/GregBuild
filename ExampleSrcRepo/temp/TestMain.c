#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "TestMain.h"

int main()
{
	void (*testHelloWorldString_fun_ptr_)(void) = &testHelloWorldString;

	(*testHelloWorldString_fun_ptr_)();

	if(result())
	{
		printf("All Tests Passed Successfully\n");
		exit(0);
	}
	else
	{
		printf("\nTests Failed\n");
	}
	exit(1);
}
