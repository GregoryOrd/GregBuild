#ifndef TEST_DEFINTIONS_H
#define TEST_DEFINITIONS_H

#define TEST_NAME_TRIM_FRONT_OFFSET 5
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE 2
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE 3

#include <dirent.h>
#include <stdbool.h>

#include "FileStructureDefs.h"

typedef struct TestCaseCheckStruct
{
  bool isTestCase;
  char commentsRemovedTestName[255];
} TestCaseCheckStruct;

typedef struct LineMetrics {
  int numSpaces;
  int numLeftBrackets;
  int numRightBrackets;
  int leftBracketIndex;
  int rightBracketIndex;
  int length;
  int expectedLeftBracketIndex;
  int expectedRightBracketIndex;
  char commentsRemovedTestName[WINDOWS_MAX_PATH_LENGTH];
} LineMetrics;

typedef struct LineAnalysisResults {
  bool correctStartOfLine;
  bool singleSpaceBetweenBoolAndTestName;
  bool hasSpecialCharacters;
  bool correctSpaces;
  bool correctBracketCount;
  bool correctBracketPosition;
  bool correctBrackets;
} LineAnalysisResults;

#ifdef __cplusplus
extern "C" {
#endif  

bool isTestDir(const char *dirName);
bool isTestFile(const struct dirent *fileOrSubDirectory);
bool isSourceFile(const char *fileOrSubDirectory);
bool isObjectFileFromSourceFile(const char* filename);
LineMetrics gatherLineMetrics(const char* line);
LineMetrics analyzeLineMetrics(LineMetrics metrics, const char* line);
LineAnalysisResults determineResults(const LineMetrics metrics, const char *line);
bool lineHasSpecialCharacters(const LineMetrics metrics, const char *line);
LineMetrics initLineMetrics();
LineAnalysisResults initLineAnalysisResults();
TestCaseCheckStruct isTestCaseDefinition(const char *line);
void trimTestName(char* testName, bool hadComments);
bool theCurlyBraceIsOnTheSameLineAsTheTestName(const char *testName,
                                               int initialLength);
bool isSpecialCharacter(const char c);

#ifdef __cplusplus
}
#endif  

#endif