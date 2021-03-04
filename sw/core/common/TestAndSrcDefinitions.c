#include "TestAndSrcDefinitions.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileStructureDefs.h"

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

bool firstTokenIsCorrect(const char* token);
bool secondTokenIsCorrect(const char* token);
void populateSecondTokenMetrics(SecondTokenMetrics* metrics, char* token);
bool metricsPointerFound(char* pointer);
void initSecondTokenMetrics(SecondTokenMetrics* metrics);
bool nothingAfterBracketsInSecondToken(SecondTokenMetrics* metrics);
bool bracketsFound(SecondTokenMetrics* metrics);
bool correctSecondTokenAfterBrackets(SecondTokenMetrics* metrics, char* token);
bool correctToken(int tokenCount, const char* token);
bool allTokensAreCorrectTestFormat(char* token, char* temp);

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

bool isObjectFileFromSourceFile(const char* filename)
{
   char lower[WINDOWS_MAX_PATH_LENGTH];
   lowerString(lower, filename);
   bool isOFile = (filename[strlen(filename) - 2] == '.') && (filename[strlen(filename) - 1] == 'o');
   bool result = (strncmp(lower, "test", 4) != 0 && isOFile);
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

void trimTestName(char* testName)
{
   int endOffset = 0;
   if (theCurlyBraceIsOnTheSameLineAsTheTestName(testName, strlen(testName)))
   {
      endOffset = TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE;
   }
   else
   {
      endOffset = TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE;
   }
   char temp[WINDOWS_MAX_PATH_LENGTH] = "";
   clearString(temp);
   for (int i = TEST_NAME_TRIM_FRONT_OFFSET; i < strlen(testName) - endOffset; i++)
   {
      temp[i - TEST_NAME_TRIM_FRONT_OFFSET] = testName[i];
      temp[i - TEST_NAME_TRIM_FRONT_OFFSET + 1] = '\0';
   }
   clearString(testName);

   strcpy(testName, temp);
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