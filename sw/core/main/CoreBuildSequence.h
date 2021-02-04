#ifndef CORE_BUILD_SEQUENCE_H
#define CORE_BUILD_SEQUENCE_H

#include "../application/CompileAndLinkCommands.h"
#include "../common/BuildSequenceStep.h"
#include "../application/RunTests.h"
#include "../common/GregBuildConstants.h"
#include "../fileSystemRecursion/FileOperations.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../testMainWriting/TestMainWriter.h"

typedef struct BuildSequenceStepInfo
{
    const char* description;
    const char* optionText;
    bool* flagValue;
    void* function_ptr;
    const char* functionName;
} BuildSequenceStepInfo;

BuildSequenceStepInfo coreBuildSequenceInfo[8] = 
{
    {
        NULL_COMMAND_LINE_DESCRIPTION, 
        NULL_COMMAND_LINE_OPTION_TEXT, 
        (bool*)NULL_COMMAND_LINE_FLAG_VALUE, 
        loadTestsAndSourceFiles, 
        "loadTestsAndSourceFiles"
    },
    {
        NULL_COMMAND_LINE_DESCRIPTION, 
        NULL_COMMAND_LINE_OPTION_TEXT, 
        (bool*)NULL_COMMAND_LINE_FLAG_VALUE, 
        compileIntoTempObjectFiles, 
        "compileIntoTempObjectFiles"
    },
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        (bool*)NO_TEST_FLAG_VALUE, 
        linkObjectFilesWithGregTestDllToMakeProjectTestDll, 
        "linkObjectFilesWithGregTestDllToMakeProjectTestDll"
    },
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        (bool*)NO_TEST_FLAG_VALUE, 
        writeTestsToTestMain, 
        "writeTestsToTestMain"
    },
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        (bool*)NO_TEST_FLAG_VALUE, 
        createTestMainExecutableFromProjectDllAndGregTestDll, 
        "createTestMainExecutableFromProjectDllAndGregTestDll"
    }, 
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        (bool*)NO_TEST_FLAG_VALUE, 
        runTestsWithExitStatusCheck, 
        "runTestsWithExitStatusCheck"
    },
    {
        NULL_COMMAND_LINE_DESCRIPTION, 
        NULL_COMMAND_LINE_OPTION_TEXT, 
        (bool*)NULL_COMMAND_LINE_FLAG_VALUE, 
        compileObjectFilesIntoProjectExecutable, 
        "compileObjectFilesIntoProjectExecutable"
    },
    {
        DELETE_TEMP_DIR_DESCRIPTION, 
        DELETE_TEMP_DIR_OPTION_TEXT, 
        (bool*)DELETE_TEMP_DIR_FLAG_VALUE, 
        removeTempDir, 
        "removeTempDir"
    }
};

#endif