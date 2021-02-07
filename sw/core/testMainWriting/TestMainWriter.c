#include "TestMainWriter.h"

#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../common/GregBuildConstants.h"

int writeTestsToTestMain(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   writeToTestMainC(testFiles);
   writeToTestMainH(testFiles);
   return 0;
}

void writeToTestMainC(const TestFileList* testFiles)
{
   int size = sizeOfTestMainC(testFiles->totalNumTestCases);
   char contents[size];
   contents[0] = '\0';
   populateTestMainCContents(contents, testFiles);
   char testMainC[WINDOWS_MAX_PATH_LENGTH] = TEMP_DIR;
   strcat(testMainC, "/TestMain.c");
   writeToFile(testMainC, contents);
}

void populateTestMainCContents(char* contents, const TestFileList* testFiles)
{
   addTestMainCIncludes(contents);
   strcat(contents, "int main()\n{\n");

   // The Function Pointer Definitions and Calls are added seperately so that all
   // definitions
   // are written to the file before the first call
   addTestMainCFunctionPointerDefinitions(contents, testFiles);
   addTestMainCFunctionPointerCalls(contents, testFiles);

   addTestMainCResultsCheckAndExits(contents);
}

void addTestMainCIncludes(char* main)
{
   strcat(main, "#include <stdbool.h>\n");
   strcat(main, "#include <stdio.h>\n");
   strcat(main, "#include <stdlib.h>\n");
   strcat(main, "#include \"TestMain.h\"\n\n");
}

void addTestMainCFunctionPointerDefinitions(char* main, const TestFileList* testFiles)
{
   for (int fileIndex = 0; fileIndex < testFiles->size; fileIndex++)
   {
      const TestFile* file = &testFiles->files[fileIndex];
      addTestMainCFunctionPointerDefinitionsForSpecificFile(main, file->numTestCases, file->cases);
   }
}

void addTestMainCFunctionPointerCalls(char* main, const TestFileList* testFiles)
{
   for (int fileIndex = 0; fileIndex < testFiles->size; fileIndex++)
   {
      const TestFile* file = &testFiles->files[fileIndex];
      addTestMainCFunctionPointerCallsForSpecificFile(main, file->numTestCases, file->cases);
   }
}

void addTestMainCFunctionPointerDefinitionsForSpecificFile(char* main, int numTests, const TestCase* cases)
{
   for (int i = 0; i < numTests; i++)
   {
      const char* testName = cases[i].testName;
      strcat(main, "\tvoid (*");
      strcat(main, testName);
      strcat(main, "_fun_ptr_)(void) = &");
      strcat(main, testName);
      strcat(main, ";\n");
   }
   strcat(main, "\n");
}

void addTestMainCFunctionPointerCallsForSpecificFile(char* main, int numTests, const TestCase* cases)
{
   for (int i = 0; i < numTests; i++)
   {
      const char* testName = cases[i].testName;
      strcat(main, "\t(*");
      strcat(main, testName);
      strcat(main, "_fun_ptr_)();\n");
   }
   strcat(main, "\n");
}

void addTestMainCResultsCheckAndExits(char* main)
{
   strcat(main, "\tif(result())\n");
   strcat(main, "\t{\n");
   strcat(main, "\t\tprintf(\"All Tests Passed Successfully\\n\");\n");
   strcat(main, "\t\texit(0);\n");
   strcat(main, "\t}\n");
   strcat(main, "\telse\n");
   strcat(main, "\t{\n");
   strcat(main, "\t\tprintf(\"\\nTests Failed\\n\");\n");
   strcat(main, "\t}\n");
   strcat(main, "\texit(1);\n");
   strcat(main, "}\n");
}

void writeToTestMainH(const TestFileList* testFiles)
{
   int size = sizeOfTestMainH(testFiles->totalNumTestCases);
   char contents[size];
   contents[0] = '\0';
   writeTestMainHGuardsAndDllDefine(contents);
   writeTestMainHGregTestDllImports(contents);
   writeTestMainHTestCaseDllImports(contents, testFiles);
   writeTestMainHEnd(contents);
   char testMainH[WINDOWS_MAX_PATH_LENGTH] = TEMP_DIR;
   strcat(testMainH, "/TestMain.h");
   writeToFile(testMainH, contents);
}

void writeTestMainHGuardsAndDllDefine(char* contents)
{
   strcat(contents, "#ifndef TEST_MAIN_H\n");
   strcat(contents, "#define TEST_MAIN_H\n\n");
   strcat(contents, "#define DllImport __declspec(dllimport)\n\n");
}

void writeTestMainHGregTestDllImports(char* contents)
{
   strcat(contents, "//From GregTest\n");
   strcat(contents, "DllImport bool result();\n\n");
}

void writeTestMainHTestCaseDllImports(char* contents, const TestFileList* testFiles)
{
   strcat(contents, "//Test Cases Found Throughout the Repo\n");
   if (testFiles->size == 0)
   {
      strcat(contents, "//No Test Files Found\n");
   }
   int numTotalTestCases = 0;
   for (int fileIndex = 0; fileIndex < testFiles->size; fileIndex++)
   {
      const TestFile* file = &testFiles->files[fileIndex];
      numTotalTestCases = numTotalTestCases + file->numTestCases;
      writeTestMainHTestCaseDllImportsForSpecificFile(contents, file->numTestCases, file->cases);
   }
   if (numTotalTestCases == 0 && testFiles->size != 0)
   {
      strcat(contents, "//No Test Cases Found\n");
   }
}

void writeTestMainHTestCaseDllImportsForSpecificFile(char* contents, int numTests, const TestCase* cases)
{
   for (int i = 0; i < numTests; i++)
   {
      const char* testName = cases[i].testName;
      strcat(contents, "DllImport void ");
      strcat(contents, testName);
      strcat(contents, "();\n");
   }
}

void writeTestMainHEnd(char* contents) { strcat(contents, "\n#endif\n"); }

int sizeOfTestMainC(int numTests)
{
   int maxTestCaseNameLength = WINDOWS_MAX_PATH_LENGTH;
   int perTestIncrement = (3 * maxTestCaseNameLength) + 47;

   int size = TEST_MAIN_C_BASE_SIZE;
   size += (numTests * perTestIncrement);
   return size;
}

int sizeOfTestMainH(int numTests)
{
   int maxTestCaseNameLength = WINDOWS_MAX_PATH_LENGTH;
   int perTestIncrement = maxTestCaseNameLength + 18;

   int size = TEST_MAIN_H_BASE_SIZE;
   size += (numTests * perTestIncrement);
   return size;
}

void writeToFile(const char* filename, const char* contents)
{
   FILE* file;
   file = fopen(filename, "w");
   int writeResult = fputs(contents, file);
   if (writeResult == EOF)
   {
      printf("Error writing test cases to %s\n", filename);
      exit(1);
   }
   fclose(file);
}