#include "ObjAndTempDefs.h"

#include <string.h>

#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "FileStructureDefs.h"
#include "GregBuildConstants.h"
#include "global/GlobalVariables.h"

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
   int length = strlen(filePath) - 1;
   bool isCpp = filePath[length] == 'p' && filePath[length - 1] == 'p' && filePath[length - 2] == 'c' && filePath[length - 3] == '.';

   int offset = 2;
   char reversedObjectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   reversedObjectFileName[0] = 'o';
   reversedObjectFileName[1] = '.';
   bool pastExtension = false;

   int loopStart = length - 1;
   if (isCpp)
   {
      loopStart = length - 3;
   }

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
      else if (filePath[i] == '.')
      {
         pastExtension = true;
      }
   }
   reverseString(objectFileName, reversedObjectFileName);
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