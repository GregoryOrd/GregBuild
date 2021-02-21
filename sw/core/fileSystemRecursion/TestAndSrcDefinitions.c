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
   LineMetrics metrics = gatherLineMetrics(line);
   metrics = analyzeLineMetrics(metrics, line);
   LineAnalysisResults results = determineResults(metrics, line);

   bool isTestCase = results.correctStartOfLine && results.correctSpaces && results.correctBrackets && !results.hasSpecialCharacters;

   return isTestCase;
}

LineMetrics gatherLineMetrics(const char* line)
{
   LineMetrics metrics = initLineMetrics();
   const char* currentPtr = line;
   while (*currentPtr != '\0' && *currentPtr != '\n')
   {
      if (*currentPtr == ' ')
      {
         metrics.numSpaces++;
      }
      else if (*currentPtr == '(')
      {
         metrics.numLeftBrackets++;
         metrics.leftBracketIndex = metrics.length;
      }
      else if (*currentPtr == ')')
      {
         metrics.numRightBrackets++;
         metrics.rightBracketIndex = metrics.length;
      }
      metrics.length++;
      currentPtr++;
   }
   return metrics;
}

LineMetrics analyzeLineMetrics(LineMetrics metrics, const char* line)
{
   // Line will have form:
   // testSomething() or testSomthing(){
   metrics.expectedLeftBracketIndex = metrics.length - 3;
   if (theCurlyBraceIsOnTheSameLineAsTheTestName(line, metrics.length))
   {
      metrics.expectedLeftBracketIndex--;
   }
   metrics.expectedRightBracketIndex = metrics.expectedLeftBracketIndex + 1;
   return metrics;
}

LineAnalysisResults determineResults(const LineMetrics metrics, const char* line)
{
   LineAnalysisResults results = initLineAnalysisResults();

   results.correctStartOfLine = strncmp(line, "void test", 9) == 0;
   results.singleSpaceBetweenBoolAndTestName = (strstr(line, " ") == &line[4]);
   results.correctSpaces = results.singleSpaceBetweenBoolAndTestName && metrics.numSpaces == 1;

   results.correctBracketCount = metrics.numLeftBrackets == 1 && metrics.numRightBrackets == 1;
   results.correctBracketPosition = metrics.leftBracketIndex == metrics.expectedLeftBracketIndex && metrics.rightBracketIndex == metrics.expectedRightBracketIndex;
   results.correctBrackets = results.correctBracketCount && results.correctBracketPosition;

   if (results.correctStartOfLine && results.correctSpaces && results.correctBrackets)
   {
      results.hasSpecialCharacters = lineHasSpecialCharacters(metrics, line);
   }
   return results;
}

bool lineHasSpecialCharacters(const LineMetrics metrics, const char* line)
{
   for (int i = 0; i < metrics.length; i++)
   {
      if (isSpecialCharacter(line[i]))
      {
         printf(
             "\nGregTest does not accept test cases with special characters in "
             "the name of the test.\n");
         printf("%s\n\n", line);
         return true;
      }
   }

   return false;
}

LineMetrics initLineMetrics()
{
   LineMetrics metrics;
   metrics.numSpaces = 0;
   metrics.numLeftBrackets = 0;
   metrics.numRightBrackets = 0;
   metrics.leftBracketIndex = 0;
   metrics.rightBracketIndex = 0;
   metrics.length = 0;
   metrics.expectedLeftBracketIndex = 0;
   metrics.expectedRightBracketIndex = 0;
   return metrics;
}

LineAnalysisResults initLineAnalysisResults()
{
   LineAnalysisResults results;
   results.correctStartOfLine = false;
   results.singleSpaceBetweenBoolAndTestName = false;
   results.hasSpecialCharacters = false;
   results.correctSpaces = false;
   results.correctBracketCount = false;
   results.correctBracketPosition = false;
   results.correctBrackets = false;
   return results;
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
   char temp[WINDOWS_MAX_PATH_LENGTH];
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

bool theCurlyBraceIsOnTheSameLineAsTheTestName(const char* testName, int initialLength) { return testName[initialLength - 2] == '{'; }

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