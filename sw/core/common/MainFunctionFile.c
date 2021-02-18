#include "MainFunctionFile.h"

#include <string.h>

#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "MainFunctionFile_Value.h"

void setMainFunctionFile(const char* filepath)
{
   strcpy(mainFunctionFilePath_, filepath);
   char temp[WINDOWS_MAX_PATH_LENGTH] = "";
   for (int i = strlen(filepath) - 1; i > -1; i--)
   {
      int index = strlen(filepath) - 1 - i;
      temp[index] = filepath[i];
      if (filepath[i] == '\\' || filepath[i] == '/')
      {
         break;
      }
   }
   reverseString(mainFunctionFileName_, temp);
   strcpy(mainFunctionObjectFileName_, mainFunctionFileName_);
   mainFunctionObjectFileName_[strlen(mainFunctionObjectFileName_) - 1] = 'o';
}

const char* mainFunctionFilePath() { return mainFunctionFilePath_; }
const char* mainFunctionFileName() { return mainFunctionFileName_; }
const char* mainFunctionObjectFileName() { return mainFunctionObjectFileName_; }