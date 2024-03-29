#ifndef TEST_DEFINTIONS_H
#define TEST_DEFINITIONS_H

#define TEST_NAME_TRIM_FRONT_OFFSET 5
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE 3
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE 4

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

#ifdef __cplusplus
extern "C" {
#endif  

bool isTestDir(const char *dirName);
bool isTestFile(const struct dirent *fileOrSubDirectory);
bool isSourceFile(const char *fileOrSubDirectory);
bool isObjectFileFromSourceFile(const char* filename);

#ifdef __cplusplus
}
#endif  

#endif