#include "CompileAndLinkCommands.h"

#include "GregBuildConstants.h"
#include "../main/GregBuildMain.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ExternalProgramExecution.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/CommandLineExecutables.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compileIntoTempObjectFiles(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath)
{
    exitIfPreviousStepFailed(previousStepFailed);

    int testFilesSize = 0;
    if(testFiles != NULL)
    {
        testFilesSize = testFiles->size;
    }

    ArgList* gccArgs = (ArgList*)malloc(sizeof(ArgList));
    gccArgs->size = testFilesSize + sourceFiles->size + 3;
    gccArgs->args = (char**)malloc(gccArgs->size * sizeof(char*));

    ArgList* mvArgs = (ArgList*)malloc(sizeof(ArgList));
    mvArgs->size = testFilesSize + sourceFiles->size + 3;
    mvArgs->args = (char**)malloc(mvArgs->size * sizeof(char*));

    populateArgsFor_compileIntoTempObjectFiles(tempObjectFiles, gccArgs, mvArgs, testFiles, sourceFiles);
    forkAndRunChildProcess(gcc, gccArgs->args);
    forkAndRunChildProcess(mv, mvArgs->args);

    freeArgList(gccArgs);
    free(mvArgs);
    return 0;
}

void populateArgsFor_compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, ArgList* gccArgs, ArgList* mvArgs, TestFileList* testFiles, SourceFileList* sourceFiles)
{
    gccArgs->args[0] = gcc;
    gccArgs->args[1] = "-c";
    gccArgs->args[gccArgs->size-1] = NULL;

    mvArgs->args[0] = mv;
    mvArgs->args[mvArgs->size-2] = TEMP_DIR;
    mvArgs->args[mvArgs->size-1] = NULL;

    int argIndex = 0;
    if(testFiles != NULL)
    {
        getArgsForTestFiles(tempObjectFiles, &argIndex, testFiles, gccArgs, mvArgs);
    }
    getArgsForSourceFiles(tempObjectFiles, &argIndex, sourceFiles, gccArgs, mvArgs);
}

int linkObjectFilesWithGregTestDllToMakeProjectTestDll(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath)
{
    exitIfPreviousStepFailed(previousStepFailed);

    ArgList* gccArgs = (ArgList*)malloc(sizeof(ArgList));
    gccArgs->size = tempObjectFiles->size + 7;
    gccArgs->args = (char**)malloc(gccArgs->size * sizeof(char*));

    gccArgs->args[0] = gcc;
    gccArgs->args[1] = "-shared";
    gccArgs->args[2] = "-o";
    gccArgs->args[3] = TEMP_TEST_PROJECT_DLL;
    for(int i = 0; i < tempObjectFiles->size; i++)
    {
        gccArgs->args[i + 4] = (&tempObjectFiles->files[i])->name;
    }
    gccArgs->args[gccArgs->size-3] = "-L./";
    gccArgs->args[gccArgs->size-2] = LIB_GREG_TEST_DLL;
    gccArgs->args[gccArgs->size-1] = NULL;

    forkAndRunChildProcess(gcc, gccArgs->args);

    freeArgList(gccArgs);
    return 0;
}

int createTestMainExecutableFromProjectDllAndGregTestDll(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath)
{
    exitIfPreviousStepFailed(previousStepFailed);
    char * const argv[] = {gcc, "-o", TEMP_TEST_MAIN, TEMP_TEST_MAIN_C, "-L./", TEMP_TEST_PROJECT_DLL, LIB_GREG_TEST_DLL, NULL};
    return forkAndRunChildProcess(gcc, argv); 
}

int compileObjectFilesIntoProjectExecutable(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath)
{
    exitIfPreviousStepFailed(previousStepFailed);

    ArgList* gccArgs = (ArgList*)malloc(sizeof(ArgList));
    gccArgs->size = numObjectFilesFromSource(tempObjectFiles) + 4;
    gccArgs->args = (char**)malloc(gccArgs->size * sizeof(char*));

    gccArgs->args[0] = gcc;
    int numObjectFilesFromSourceAddedToArgsList = 0;
    for(int i = 0; i < tempObjectFiles->size; i++)
    {
        ObjectFile* file = &tempObjectFiles->files[i];
        if(file->isFromSource)
        {
            gccArgs->args[numObjectFilesFromSourceAddedToArgsList + 1] = file->name;
            numObjectFilesFromSourceAddedToArgsList++;
        }
    }
    gccArgs->args[gccArgs->size-3] = "-o";
    gccArgs->args[gccArgs->size-2] = PROJECT_EXE;
    gccArgs->args[gccArgs->size-1] = NULL;

    makeDir(DIST);
    int retval = forkAndRunChildProcess(gcc, gccArgs->args);   
    freeArgList(gccArgs);
    if(retval == 0)
    {
        printf("\nBuild Successful!\n");
    }
    else
    {
        printf("Error Compiling the Code After Tests Completed\n");
    }
    return retval;
}

void getArgsForTestFiles(ObjectFileList* tempObjectFiles, int* argIndex, TestFileList* testFiles, ArgList* gccArgs, ArgList* mvArgs)
{
    char* objectFileName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
    int gccFileArgOffset = 2;
    int mvFileArgOffset = 1;
    int testFileIndex = 0;
    while(testFileIndex < testFiles->size)
    {
        clearString(objectFileName);
        determineObjectFileName(objectFileName, testFiles->files[testFileIndex].name);
        addTempObjectFileToList(tempObjectFiles, objectFileName, false);
        gccArgs->args[*argIndex + gccFileArgOffset] = testFiles->files[testFileIndex].name;
        mvArgs->args[*argIndex + mvFileArgOffset] = (char*)malloc(strlen(objectFileName) * sizeof(char));
        strcpy(mvArgs->args[*argIndex + mvFileArgOffset], objectFileName);
        (*argIndex)++;
        testFileIndex++;
    } 
    free(objectFileName);
}

void getArgsForSourceFiles(ObjectFileList* tempObjectFiles, int* argIndex, SourceFileList* sourceFiles, ArgList* gccArgs, ArgList* mvArgs)
{
    char* objectFileName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
    int gccFileArgOffset = 2;
    int mvFileArgOffset = 1;
    int sourceFileIndex = 0;
    while(sourceFileIndex < sourceFiles->size)
    {
        clearString(objectFileName);
        determineObjectFileName(objectFileName, sourceFiles->files[sourceFileIndex].name);
        addTempObjectFileToList(tempObjectFiles, objectFileName, true);
        gccArgs->args[*argIndex + gccFileArgOffset] = sourceFiles->files[sourceFileIndex].name;
        mvArgs->args[*argIndex + mvFileArgOffset] = (char*)malloc(strlen(objectFileName) * sizeof(char));
        strcpy(mvArgs->args[*argIndex + mvFileArgOffset], objectFileName);
        (*argIndex)++;
        sourceFileIndex++;
    }
    free(objectFileName);
}

void determineObjectFileName(char* objectFileName, const char* filePath)
{
    int length = strlen(filePath) - 1;
    int offset = 2;
    char* reversedObjectFileName = (char*)malloc(length * sizeof(char));
    reversedObjectFileName[0] = 'o';
    reversedObjectFileName[1] = '.';
    bool pastExtension = false;

    for(int i = length - 1; i > 0; i--)
    {
        if(filePath[i] == '\\' || filePath[i] == '/')
        {
            break;
        }
        else if(filePath[i] != '.')
        {
            reversedObjectFileName[offset] = filePath[i];
            reversedObjectFileName[offset + 1] = '\0';
            offset++;
        }
        else if(filePath[i] == '.')
        {
            pastExtension = true;
        }
    }
    reverseString(objectFileName, reversedObjectFileName);
    free(reversedObjectFileName);
}

void addTempObjectFileToList(ObjectFileList* list, char* filename, bool isFromSource)
{
    int tempObjectFileLength = strlen(TEMP_DIR) + strlen("\\") + strlen(filename);
    char* tempObjectFile = (char*)malloc(tempObjectFileLength * sizeof(char));
    clearString(tempObjectFile);
    strcat(tempObjectFile, TEMP_DIR);
    strcat(tempObjectFile, "\\");
    strcat(tempObjectFile, filename);

    list->files = (ObjectFile*)realloc(list->files, ((list->size + 1) * sizeof(ObjectFile)));
    list->files[list->size].name = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*)); 
    strcpy(list->files[list->size].name, tempObjectFile);
    list->files[list->size].isFromSource = isFromSource;
    list->size++; 

    free(tempObjectFile);
}

int numObjectFilesFromSource(ObjectFileList* tempObjectFiles)
{
    int count = 0;
    for(int i = 0; i < tempObjectFiles->size; i++)
    {
        if((&tempObjectFiles->files[i])->isFromSource)
        {
            count++;
        }
    }
    return count;
}

void freeArgList(ArgList* argList)
{
    free(argList->args);
    free(argList);
}