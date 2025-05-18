#include "TestMainWriter.h"

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/CommandLineExecutables.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/GregBuildConstants.h"
#include "../common/ObjAndTempDefs.h"
#include "../common/global/GlobalVariables.h"
#include "../compiler/CompileAndLinkHelpers.h"

//////////////////////////////////////////////////////////////////////
//              Private Data and Function Prototypes                //
//////////////////////////////////////////////////////////////////////

static const int BUFFER_SIZE = 4096;
static void getTestNamesFromTestObjectFile(TestFileList* testFiles, const char* objectFileName, int index, char* srcFileName);
static void getTestNameFromThirdToken(char* token, char* temp, char* thirdToken, char* srcFileName);
static void readFileFromPipeBuffer(TestFileList* testFiles, FILE* pipe, int index, char* srcFileName);
static void populateTestCases(TestFileList* testFiles, int index);
static void nmAndGrepCommand(char* command, const char* objectFileName);
static void addTestCase(char* testCaseName, TestFileList* testFile, int index);
static void reallocTestCasesList(TestFileList* testFiles, int index);
static void demangleToken(char* token);
static void getDemangleCommand(char* command, const char* token);

//////////////////////////////////////////////////////////////////////
//              Function Implementation Section                     //
//////////////////////////////////////////////////////////////////////

int writeTestsToTestMain(TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   for (int i = 0; i < testFiles->size; i++)
   {
      populateTestCases(testFiles, i);
   }

   if (tempObjectFiles->size > 0)
   {
      writeToTestMainC(testFiles);
      writeToTestMainH(testFiles);
   }
   return 0;
}

void populateTestCases(TestFileList* testFiles, int index)
{
   char objectFileName[WINDOWS_MAX_PATH_LENGTH];
   determineObjectFilePathUsingListType(TEST_FILE_LIST_TYPE, objectFileName, hostCompiler(), testFiles, index);

   char srcFileName[WINDOWS_MAX_PATH_LENGTH];
   strcpy(srcFileName, testFiles->files[index].name);

   getTestNamesFromTestObjectFile(testFiles, objectFileName, index, srcFileName);
}

void getTestNamesFromTestObjectFile(TestFileList* testFiles, const char* objectFileName, int index, char* srcFileName)
{
   char command[WINDOWS_MAX_PATH_LENGTH + 8] = "";
   nmAndGrepCommand(command, objectFileName);

   FILE* pipe = popen(command, "r");
   readFileFromPipeBuffer(testFiles, pipe, index, srcFileName);
   pclose(pipe);
}

void nmAndGrepCommand(char* command, const char* objectFileName)
{
   strcat(command, nm);
   strcat(command, " ");
   strcat(command, objectFileName);
   strcat(command, " | ");
   strcat(command, grep);
   strcat(command, " ");
   strcat(command, "T");
}

void readFileFromPipeBuffer(TestFileList* testFiles, FILE* pipe, int index, char* srcFileName)
{
   char buffer[BUFFER_SIZE];
   while (fgets(buffer, BUFFER_SIZE, pipe))
   {
      char* token;
      char* temp = strdup(buffer);
      char* testCaseName = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
      getTestNameFromThirdToken(token, temp, testCaseName, srcFileName);
      addTestCase(testCaseName, testFiles, index);

      free(testCaseName);
      free(temp);
   }
}

void addTestCase(char* testCaseName, TestFileList* testFiles, int index)
{
   if (!stringsAreEqual(testCaseName, ""))
   {
      reallocTestCasesList(testFiles, index);
      int testCaseIndex = testFiles->files[index].numTestCases - 1;
      strcpy(testFiles->files[index].cases[testCaseIndex].testName, testCaseName);
      testFiles->totalNumTestCases++;
   }
}

void reallocTestCasesList(TestFileList* testFiles, int index)
{
   testFiles->files[index].cases = realloc(testFiles->files[index].cases, ((testFiles->files[index].numTestCases + 1) * sizeof(TestCase)));
   testFiles->files[index].numTestCases++;
}

void getTestNameFromThirdToken(char* token, char* temp, char* testCaseName, char* srcFileName)
{
   bool secondTokenIsUpperT = false;
   int tokenCount = 0;
   while (token = strtok_r(temp, " \n", &temp))
   {
      if (tokenCount == 1)
      {
         secondTokenIsUpperT = stringsAreEqual(token, "T");
      }
      else if (tokenCount == 2 && secondTokenIsUpperT)
      {
         if (isCpp(srcFileName))
         {
            demangleToken(token);
            removeFunctionBrackets(token);
         }
         strcpy(testCaseName, token);
      }
      tokenCount++;
   }
}

void demangleToken(char* token)
{
   char command[WINDOWS_MAX_PATH_LENGTH + 8] = "";
   getDemangleCommand(command, token);

   FILE* pipe = popen(command, "r");
   char buffer[BUFFER_SIZE];
   fgets(buffer, BUFFER_SIZE, pipe);
   pclose(pipe);
   strcpy(token, buffer);
   removeTrailingNewLine(token);
}

void getDemangleCommand(char* command, const char* token)
{
   strcat(command, cppfilt_demangle);
   strcat(command, " ");
   strcat(command, token);
}

void writeToTestMainC(const TestFileList* testFiles)
{
   int size = sizeOfTestMainC(testFiles->totalNumTestCases);
   char* contents = (char*)calloc(size, sizeof(char));
   char* testMainC = (char*)calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));

   populateTestMainCContents(contents, testFiles);
   strcpy(testMainC, TEMP_DIR);
   strcat(testMainC, "/TestMain.c");
   writeToFile(testMainC, contents);

   free(contents);
   free(testMainC);
}

void populateTestMainCContents(char* contents, const TestFileList* testFiles)
{
   addTestMainCIncludes(contents);
   strcat(contents, "int main()\n{\n");

   // The Function Pointer Definitions and Calls are added seperately so that all
   // definitions are written to the file before the first call
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
   char* contents = (char*)calloc(size, sizeof(char));
   char* testMainH = (char*)calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));

   writeTestMainHGuardsAndDllDefine(contents);
   writeTestMainHGregTestLibraryImports(contents);
   writeTestMainHTestCaseLibraryImports(contents, testFiles);
   writeTestMainHEnd(contents);

   strcpy(testMainH, TEMP_DIR);
   strcat(testMainH, "/TestMain.h");
   writeToFile(testMainH, contents);

   free(contents);
   free(testMainH);
}

void writeTestMainHGuardsAndDllDefine(char* contents)
{
   strcat(contents, "#ifndef TEST_MAIN_H\n");
   strcat(contents, "#define TEST_MAIN_H\n\n");
   strcat(contents, LIBRARY_IMPORT_DEFINE);
}

void writeTestMainHGregTestLibraryImports(char* contents)
{
   strcat(contents, "//From GregTest\n");
   strcat(contents, "LibraryImport bool result();\n\n");
}

void writeTestMainHTestCaseLibraryImports(char* contents, const TestFileList* testFiles)
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
      writeTestMainHTestCaseLibraryImportsForSpecificFile(contents, file->numTestCases, file->cases);
   }
   if (numTotalTestCases == 0 && testFiles->size != 0)
   {
      strcat(contents, "//No Test Cases Found\n");
   }
}

void writeTestMainHTestCaseLibraryImportsForSpecificFile(char* contents, int numTests, const TestCase* cases)
{
   for (int i = 0; i < numTests; i++)
   {
      const char* testName = cases[i].testName;
      strcat(contents, "LibraryImport void ");
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