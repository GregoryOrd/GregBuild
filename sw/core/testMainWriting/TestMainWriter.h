#ifndef TESTMAINWRITER_H
#define TESTMAINWRITER_H

#define TEST_MAIN_C_BASE_SIZE 228
#define TEST_MAIN_H_BASE_SIZE 193

#include "../common/FileStructureDefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int writeTestsToTestMain(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int previousStepFailed, const char* basePath);
void writeToTestMainC(const TestFileList* testFiles);
void populateTestMainCContents(char* contents, const TestFileList* testFiles);
void addTestMainCIncludes(char* main);
void addTestMainCFunctionPointerDefinitions(char* main, const TestFileList* testFiles);
void addTestMainCFunctionPointerCalls(char* main, const TestFileList* testFiles);
void addTestMainCFunctionPointerDefinitionsForSpecificFile(char* main, int numTests, const TestCase* cases);
void addTestMainCFunctionPointerCallsForSpecificFile(char* main, int numTests, const TestCase* cases);
void addTestMainCResultsCheckAndExits(char* main);
int sizeOfTestMainC(int numTests);

void writeToTestMainH(const TestFileList* testFiles);
void writeTestMainHGuardsAndDllDefine(char* contents);
void writeTestMainHGregTestDllImports(char* contents);
void writeTestMainHTestCaseDllImports(char* contents, const TestFileList* testFiles);
void writeTestMainHTestCaseDllImportsForSpecificFile(char* contents, int numTests, const TestCase* cases);
void writeTestMainHEnd(char* contents);
int sizeOfTestMainH(int numTests);
void writeToFile(const char* filename, const char* contents);

#endif