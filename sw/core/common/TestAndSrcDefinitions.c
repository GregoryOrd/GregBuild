#include "TestAndSrcDefinitions.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileStructureDefs.h"

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
   char* temp = strdup(line);
   char* token;
   strcpy(temp, line);

   int tokenCount = 0;
   bool correctFirstToken = false;
   bool correctSecondToken = false;
   while (token = strtok_r(temp, " \r\t\n", &temp))
   {
      if (tokenCount == 0 && stringsAreEqual(token, "void"))
      {
         correctFirstToken = true;
      }
      else if (tokenCount == 1)
      {
         bool hasBrackets = false;
         bool hasLeftBrace = false;
         bool hasRightBrace = false;
         bool hasSingleLineComment = false;
         bool hasMultiLineCommentStart = false;
         bool hasMultiLineCommentEnd = false;

         if (strstr(token, "{"))
         {
            hasLeftBrace = true;
         }

         if (strstr(token, "}"))
         {
            hasRightBrace = true;
         }

         if (strstr(token, "//"))
         {
            hasSingleLineComment = true;
         }

         if (strstr(token, "/*"))
         {
            hasMultiLineCommentStart = true;
         }

         if (strstr(token, "*/"))
         {
            hasMultiLineCommentEnd = true;
         }

         char* brackets;
         if (brackets = strstr(token, "()"))
         {
            if (!hasLeftBrace && !hasRightBrace && !hasSingleLineComment && !hasMultiLineCommentStart && !hasMultiLineCommentEnd)
            {
               char* secondLast = token + strlen(token) - 2;
               correctSecondToken = secondLast == brackets;
            }
            else if (hasLeftBrace)
            {
               char* thirdLast = token + strlen(token) - 3;
               correctSecondToken = thirdLast == brackets;
            }
         }
      }
      tokenCount++;
   }

   return correctFirstToken && correctSecondToken;
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