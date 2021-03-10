#include "TestAndSrcDefinitions.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileStructureDefs.h"
#include "../common/global/GlobalVariables.h"
#include "GregBuildConstants.h"

//////////////////////////////////////////////////////////////////////
//              Private Data and Function Prototypes                //
//////////////////////////////////////////////////////////////////////

typedef struct
{
   char* brackets;
   char* leftBrace;
   char* rightBrace;
   char* singleLineComment;
   char* multilineCommentStart;
   char* mulitilineCommentEnd;
} SecondTokenMetrics;

static bool firstTokenIsCorrect(const char* token);
static bool secondTokenIsCorrect(const char* token);
static void populateSecondTokenMetrics(SecondTokenMetrics* metrics, char* token);
static bool metricsPointerFound(char* pointer);
static void initSecondTokenMetrics(SecondTokenMetrics* metrics);
static bool nothingAfterBracketsInSecondToken(SecondTokenMetrics* metrics);
static bool bracketsFound(SecondTokenMetrics* metrics);
static bool correctSecondTokenAfterBrackets(SecondTokenMetrics* metrics, char* token);
static bool correctToken(int tokenCount, const char* token);
static bool allTokensAreCorrectTestFormat(char* token, char* temp);
static bool oneOfTheTokensIsFromSource(char* token, char* temp);

//////////////////////////////////////////////////////////////////////
//                     Function Implementations                     //
//////////////////////////////////////////////////////////////////////

bool isTestDir(const char* dirName)
{
   char lower[WINDOWS_MAX_PATH_LENGTH] = "";
   lowerString(lower, dirName);
   return strstr(lower, "/test") != NULL;
}

bool isTestFile(const struct dirent* fileOrSubDirectory)
{
   char lower[WINDOWS_MAX_PATH_LENGTH];
   lowerString(lower, fileOrSubDirectory->d_name);
   bool result = (strncmp(lower, "test", 4) == 0 && (strstr(lower, ".c") != NULL || strstr(lower, ".cpp") != NULL));
   return result;
}

bool isSourceFile(const char* filename)
{
   char lower[WINDOWS_MAX_PATH_LENGTH];
   bool isCFile = (filename[strlen(filename) - 2] == '.') && (filename[strlen(filename) - 1] == 'c');
   bool isCPPFile = strlen(filename) > 4 && (filename[strlen(filename) - 4] == '.') && (filename[strlen(filename) - 3] == 'c') && (filename[strlen(filename) - 2] == 'p') &&
                    (filename[strlen(filename) - 1] == 'p');
   bool result = (strncmp(filename, "test", 4) != 0 && (isCFile || isCPPFile));
   return result;
}

bool oneOfTheTokensIsFromSource(char* token, char* temp)
{
   bool retval = false;
   while (token = strtok_r(temp, "/", &temp))
   {
      char lower[WINDOWS_MAX_PATH_LENGTH];
      lowerString(lower, token);
      bool isOFile = (token[strlen(token) - 2] == '.') && (token[strlen(token) - 1] == 'o');
      bool result = (strncmp(lower, "test", 4) != 0 && isOFile);
      if (result)
      {
         return true;
      }
   }
   return retval;
}

bool isObjectFileFromSourceFile(const char* filename)
{
   char* token;
   char* temp = strdup(filename);

   bool result = oneOfTheTokensIsFromSource(token, temp);

   free(temp);
   return result;
}