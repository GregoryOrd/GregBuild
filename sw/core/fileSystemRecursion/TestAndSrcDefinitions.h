#ifndef TEST_DEFINTIONS_H
#define TEST_DEFINITIONS_H

#define TEST_NAME_TRIM_FRONT_OFFSET 5
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE 8
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE 9

#include <dirent.h>
#include <stdbool.h>

typedef struct LineMetrics {
  int numSpaces;
  int numLeftBrackets;
  int numRightBrackets;
  int leftBracketIndex;
  int rightBracketIndex;
  int length;
  int expectedLeftBracketIndex;
  int expectedRightBracketIndex;
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

bool isTestDir(const char *dirName);
bool isTestFile(const struct dirent *fileOrSubDirectory);
bool isSourceFile(const struct dirent *fileOrSubDirectory);
void gatherLineMetrics(LineMetrics *metrics, const char *line);
void analyzeLineMetrics(LineMetrics *metrics, const char *line);
void determineResults(LineAnalysisResults *results, const LineMetrics *metrics,
                      const char *line);
bool lineHasSpecialCharacters(const LineMetrics *metrics, const char *line);
void initLineMetrics(LineMetrics *metrics);
void initLineAnalysisResults(LineAnalysisResults *results);
bool isTestCaseDefinition(const char *line);
int testNameEndOffset(const char *testName);
bool theCurlyBraceIsOnTheSameLineAsTheTestName(const char *testName,
                                               int initialLength);
bool isSpecialCharacter(const char c);

#endif