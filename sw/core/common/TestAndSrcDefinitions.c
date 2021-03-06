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
   bool result = (strncmp(filename, "test", 4) != 0 && isCFile);
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

bool isTestCaseDefinition(const char* line)
{
   char* token;
   char* temp = strdup(line);
   strcpy(temp, line);

   bool result = allTokensAreCorrectTestFormat(token, temp);
   free(temp);
   return result;
}

bool allTokensAreCorrectTestFormat(char* token, char* temp)
{
   bool result = false;
   int tokenCount = 0;
   while (token = strtok_r(temp, " \r\t\n", &temp))
   {
      result = correctToken(tokenCount, token);
      if (!result) break;
      tokenCount++;
   }

   return result;
}

bool correctToken(int tokenCount, const char* token)
{
   switch (tokenCount)
   {
      case 0:
         return firstTokenIsCorrect(token);
         break;
      case 1:
         return secondTokenIsCorrect(token);
      default:
         return false;
   }
}

bool firstTokenIsCorrect(const char* token) { return stringsAreEqual(token, "void"); }

bool secondTokenIsCorrect(const char* token)
{
   SecondTokenMetrics* metrics = malloc(sizeof(SecondTokenMetrics));
   initSecondTokenMetrics(metrics);
   populateSecondTokenMetrics(metrics, (char*)token);
   bool result = false;

   if (bracketsFound(metrics))
   {
      result = correctSecondTokenAfterBrackets(metrics, (char*)token);
   }
   free(metrics);
   return result;
}

bool correctSecondTokenAfterBrackets(SecondTokenMetrics* metrics, char* token)
{
   if (nothingAfterBracketsInSecondToken(metrics))
   {
      char* secondLast = token + strlen(token) - 2;
      return secondLast == metrics->brackets;
   }
   else if (metrics->leftBrace)
   {
      char* thirdLast = token + strlen(token) - 3;
      return thirdLast == metrics->brackets;
   }
}

bool bracketsFound(SecondTokenMetrics* metrics) { return metricsPointerFound(metrics->brackets); }

bool nothingAfterBracketsInSecondToken(SecondTokenMetrics* metrics)
{
   return (
       !metricsPointerFound(metrics->leftBrace) && !metricsPointerFound(metrics->rightBrace) && !metricsPointerFound(metrics->singleLineComment) &&
       !metricsPointerFound(metrics->multilineCommentStart) && !metricsPointerFound(metrics->mulitilineCommentEnd));
}

void initSecondTokenMetrics(SecondTokenMetrics* metrics)
{
   metrics->brackets = NULL;
   metrics->leftBrace = NULL;
   metrics->rightBrace = NULL;
   metrics->singleLineComment = NULL;
   metrics->multilineCommentStart = NULL;
   metrics->multilineCommentStart = NULL;
}

bool metricsPointerFound(char* pointer) { return pointer != NULL; }

void populateSecondTokenMetrics(SecondTokenMetrics* metrics, char* token)
{
   metrics->brackets = strstr(token, "()");
   metrics->leftBrace = strstr(token, "{");
   metrics->rightBrace = strstr(token, "}");
   metrics->singleLineComment = strstr(token, "//");
   metrics->multilineCommentStart = strstr(token, "/*");
   metrics->mulitilineCommentEnd = strstr(token, "*/");
}

int testNameEndOffset(const char* testName)
{
   if (theCurlyBraceIsOnTheSameLineAsTheTestName(testName, strlen(testName) - 1))
   {
      return TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE;
   }
   return TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE;
}

bool theCurlyBraceIsOnTheSameLineAsTheTestName(const char* testName, int initialLength)
{
   // Using temp to get rid of valgrind message
   char temp[WINDOWS_MAX_PATH_LENGTH] = "";
   strcpy(temp, testName);
   return temp[initialLength - 2] == '{';
}

bool isSpecialCharacter(const char c)
{
   bool isCarriageReturn = c == 13;
   bool isNewLine = c == '\n';
   bool isNumber = c >= '0' && c <= '9';
   bool isUpperCaseLetter = c >= 'A' && c <= 'Z';
   bool isLowerCaseLetter = c >= 'a' && c <= 'z';
   bool isSpace = c == ' ';
   bool isLeftCurrlyBracket = c == '{';
   bool isParenthesis = (c == '(') || (c == ')');

   if (isCarriageReturn || isNewLine || isNumber || isUpperCaseLetter || isLowerCaseLetter || isSpace || isLeftCurrlyBracket || isParenthesis)
   {
      return false;
   }
   return true;
}

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
   int offset = 2;
   char reversedObjectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   reversedObjectFileName[0] = 'o';
   reversedObjectFileName[1] = '.';
   bool pastExtension = false;

   for (int i = length - 1; i > 0; i--)
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