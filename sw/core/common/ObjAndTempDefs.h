#ifndef OBJ_AND_TEMP_DEFS
#define OBJ_AND_TEMP_DEFS

#include <stdbool.h>

void determineObjectFilePathUsingListType(int listType, char* objectFileName, const char* compiler, const void* fileList, int index);
void determineObjectFileNameUsingListType(int listType, char* objectFileName, const void* fileList, int index);
void determineObjectFileName(char* objectFileName, const char* filePath);
void tempDirPathFromCompiler(char* dest, const char* compiler);
bool isCpp(const char* filePath);

#endif