#include "ObjAndTempDefs.h"

#include <string.h>

#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "FileStructureDefs.h"
#include "GregBuildConstants.h"
#include "global/GlobalVariables.h"

//////////////////////////////////////////////////////////////////////
//              Private Data and Function Prototypes                //
//////////////////////////////////////////////////////////////////////
#define DOT_o_EXTENSION_OFFSET 2
int loopStart(const char* filePath);
void loopBackwardsUntilFolderSlash(char* reversedObjectFileName, const char* filePath, int loopStart);

//////////////////////////////////////////////////////////////////////
//              Function Implementation Section                     //
//////////////////////////////////////////////////////////////////////
void determineObjectFilePathUsingListType(int listType, char* objectFileName, const char* compiler, const void* fileList, int index)
{
   if (listType == TEST_FILE_LIST_TYPE)
   {
      determineObjectFileName(objectFileName, ((TestFileList*)fileList)->files[index].name);
   }
   else
   {
      determineObjectFileName(objectFileName, ((SourceFileList*)fileList)->files[index].name);
   }
   char tempObjectFile[WINDOWS_MAX_PATH_LENGTH] = "";
   tempDirPathFromCompiler(tempObjectFile, compiler);
   strcat(tempObjectFile, DELIMITER);
   strcat(tempObjectFile, objectFileName);
   strcpy(objectFileName, tempObjectFile);
}

void determineObjectFileNameUsingListType(int listType, char* objectFileName, const void* fileList, int index)
{
   if (listType == TEST_FILE_LIST_TYPE)
   {
      determineObjectFileName(objectFileName, ((TestFileList*)fileList)->files[index].name);
   }
   else
   {
      determineObjectFileName(objectFileName, ((SourceFileList*)fileList)->files[index].name);
   }
}

void determineObjectFileName(char* objectFileName, const char* filePath)
{
   char reversedObjectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   reversedObjectFileName[0] = 'o';
   reversedObjectFileName[1] = '.';

   loopBackwardsUntilFolderSlash(reversedObjectFileName, filePath, loopStart(filePath));
   reverseString(objectFileName, reversedObjectFileName);
}

void loopBackwardsUntilFolderSlash(char* reversedObjectFileName, const char* filePath, int loopStart)
{
   int offset = DOT_o_EXTENSION_OFFSET;
   for (int i = loopStart; i > 0; i--)
   {
      if (filePath[i] == '\\' || filePath[i] == '/')
      {
         break;
      }
      else if (filePath[i] != '.')
      {
         reversedObjectFileName[offset] = filePath[i];
         reversedObjectFileName[offset + 1] = '\0';
         offset++;
      }
   }
}

bool isCpp(const char* filePath)
{
   int length = strlen(filePath);
   bool dot = filePath[length - 4] == '.';
   bool c = filePath[length - 3] == 'c';
   bool p1 = filePath[length - 2] == 'p';
   bool p2 = filePath[length - 1] == 'p';

   return dot && c && p1 && p2;
}

int loopStart(const char* filePath)
{
   int length = strlen(filePath) - 1;
   int start = length - 1;
   if (isCpp(filePath))
   {
      start = length - 3;
   }
   return start;
}

void tempDirPathFromCompiler(char* dest, const char* compiler)
{
   char hardwarePlatform[7] = "target";
   if (stringsAreEqual(compiler, hostCompiler()))
   {
      strcpy(hardwarePlatform, "host");
   }

   clearString(dest);
   strcpy(dest, TEMP_DIR);
   strcat(dest, DELIMITER);
   strcat(dest, hardwarePlatform);
}