#include "StringUtils.h"

#include <string.h>

void clearString(char* str)
{
    int length = strlen(str);
    for(int i = 0; i < length - 1; i++)
    {
        str[i] = '\0';
    }
}

void reverseString(char* dest, char* src)
{
    int srcLength = strlen(src);
    int countUp = 0;
    for(int i = srcLength - 1; i >= 0; i--)
    {
        if(src[i] != '\0')
        {
            dest[countUp] = src[i];
            countUp++;
        }
    }
    dest[countUp] = '\0';
}