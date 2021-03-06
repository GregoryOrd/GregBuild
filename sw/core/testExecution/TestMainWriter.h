#ifndef TESTMAINWRITER_H
#define TESTMAINWRITER_H

#define TEST_MAIN_C_BASE_SIZE 228
#define TEST_MAIN_H_BASE_SIZE 193

#include "../common/FileStructureDefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __WINDOWS__
#define LIBRARY_IMPORT_DEFINE "#define LibraryImport __declspec(dllimport)\n\n"
#else
#define LIBRARY_IMPORT_DEFINE "#define LibraryImport \n\n"
#endif

#ifdef __cplusplus
extern "C" {
#endif  

int writeTestsToTestMain(TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
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
void writeTestMainHGregTestLibraryImports(char* contents);
void writeTestMainHTestCaseLibraryImports(char* contents, const TestFileList* testFiles);
void writeTestMainHTestCaseLibraryImportsForSpecificFile(char* contents, int numTests, const TestCase* cases);
void writeTestMainHEnd(char* contents);
int sizeOfTestMainH(int numTests);
void writeToFile(const char* filename, const char* contents);

#ifdef __cplusplus
}
#endif  

#endif