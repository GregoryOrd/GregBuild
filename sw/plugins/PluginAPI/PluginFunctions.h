// The file is provided to be used when creating plugin dlls
// The structs here are copies of those used internally by GregBuild
// The functions declared here are open to be implemented by plugin developers
// These functions are the only functions that will be recognized by the
// GregBuild plugin loader

#ifndef PLUGIN_FUNCTIONS_H
#define PLUGIN_FUNCTIONS_H

#define NULL_COMMAND_LINE_OPTION_TEXT ""
#define NULL_COMMAND_LINE_DESCRIPTION ""
#define NULL_COMMAND_LINE_FLAG_VALUE 1

#define WINDOWS_MAX_PATH_LENGTH 260

#include <stdbool.h>

typedef struct ObjectFile {
  char *name;
  bool isFromSource;
} ObjectFile;

typedef struct ObjectFileList {
  int size;
  ObjectFile *files;
} ObjectFileList;

typedef struct SourceFile { char *name; } SourceFile;

typedef struct SourceFileList {
  int size;
  SourceFile *files;
} SourceFileList;

typedef struct TestCase { char *testName; } TestCase;

typedef struct TestFile {
  char *name;
  int numTestCases;
  TestCase *cases;
} TestFile;

typedef struct TestFileList {
  int size;
  int totalNumTestCases;
  TestFile *files;
} TestFileList;

struct CommandLineOption {
  char *optionText;
  char *description;
  bool flagValue;
} typedef CommandLineOption;

typedef struct BuildSequenceStep {
  CommandLineOption *option; // To run the function, the flagValue in here needs to be true
  int (*function_ptr)(TestFileList *testFiles, SourceFileList *sourceFiles,
                      ObjectFileList *tempObjectFiles, int errorOnPreviousStep,
                      char *basePath);
  char * functionName;
} BuildSequenceStep;

// The core GregBuild BuildSequence is as follows
// 1) loadTestAndSourceFiles()
//      - This searches through the repo recursively for .c files.
// 2) compileIntoTempObjectFiles()
//      - Compiles all of the found .c files into .o files.
// 3) linkObjectFilesWithGregTestDllToMakeProjectTestDll()
//      - Can be skipped with --no-test-build command line option
// 4) writeTestsToTestMain()
//      - Can be skipped with --no-test-build command line option
//      - Parses through the found test files to find test cases.
//      - Uses these test cases to create TestMain.c and TestMain.h
// 5) createTestMainExecutableFromProjectDllAndGregTestDll()
//      - Can be skipped with --no-test-build command line option
//      - Compiles TestMain.c with the project test dll and GregTest.dll
//      - This creates an executable with all of the tests
// 6) runTestsWithExitStatusCheck
//      - Can be skipped with --no-test-build command line option
//      - Run the test executable
//      - Check the test executbale exit status for segmentation faults
//      - If a segmentation fault occurs in the test executable, display errorOnPreviousStep
//      message
//      - Otherwise return pass or fail
// 7) compileObjectFilesIntoProjectExecutable()
//      - The source files have already been compiled into .o files at this
//      point
//      - This step just links together the .o files that were created in step 2
// 8) removeTempDir()
//      - Can be skipped with --leave-temp command line options
//      - Remove the temp directory where the .o files live
//      - Note: this step might be changed soon in the future to add smart
//      building
//        where .o files are not recompiled if the source file hasn't been
//        modified

// To return this BuildSequenceStep*, plugins are expected to dynamically
// allocate
// memory with a malloc call. GregBuild core will then take care freeing this
// memory.
// For example, a plugin implementation of one of these functions should look as
// shown below.
// Note that the printHelloWorld() function must have the correct arguments
// and should return 0 for success and 1 for failure.
/*
BuildSequenceStep* beforeLoadingTestAndSourceFiles()
{
   BuildSequenceStep* step =
   malloc(sizeof(BuildSequenceStep));
   step->option = malloc(sizeof(CommandLineOption));
   step->option->optionText = malloc(WINDOWS_MAX_PATH_LENGTH*sizeof(char));
   step->option->description = malloc(WINDOWS_MAX_PATH_LENGTH*sizeof(char));
   step->functionName = malloc(WINDOWS_MAX_PATH_LENGTH*sizeof(char));

   step->function_ptr = printHelloWorld;
   strcpy(step->functionName, "printHelloWorld"); //Should match the function_ptr name
   strcpy(step->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
   strcpy(step->option->description, NULL_COMMAND_LINE_DESCRIPTION);
   step->option->flagValue = NULL_COMMAND_LINE_FLAG_VALUE;

   return step;
}

int printHelloWorld(TestFileList* testFiles, SourceFileList* sourceFiles,
ObjectFileList* tempObjectFiles, int errorOnPreviousStep, char* basePath)
{
   printf("Hello World From The Best Plugin\n");
   return 0; // 0 for Success, 1 for Failure
}
*/

BuildSequenceStep *before_loadTestsAndSourceFiles();
BuildSequenceStep *after_loadTestsAndSourceFiles();

BuildSequenceStep *before_compileIntoTempObjectFiles();
BuildSequenceStep *after_compileIntoTempObjectFiles();

BuildSequenceStep *before_linkObjectFilesWithGregTestDllToMakeProjectTestDll();
BuildSequenceStep *after_linkObjectFilesWithGregTestDllToMakeProjectTestDll();

BuildSequenceStep *before_writeTestsToTestMain();
BuildSequenceStep *after_writeTestsToTestMain();

BuildSequenceStep *before_createTestMainExecutableFromProjectDllAndGregTestDll();
BuildSequenceStep *after_createTestMainExecutableFromProjectDllAndGregTestDll();

BuildSequenceStep *before_runTestsWithExitStatusCheck();
BuildSequenceStep *after_runTestsWithExitStatusCheck();

BuildSequenceStep *before_compileObjectFilesIntoProjectExecutable();
BuildSequenceStep *after_compileObjectFilesIntoProjectExecutable();

BuildSequenceStep *before_removeTempDir();
BuildSequenceStep *after_removeTempDir();

#endif