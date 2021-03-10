#ifndef CORE_BUILD_SEQUENCE_H
#define CORE_BUILD_SEQUENCE_H

#include "../compiler/CompileAndLinkCommands.h"
#include "../common/BuildSequenceStep.h"
#include "../testExecution/RunTests.h"
#include "../common/GregBuildConstants.h"
#include "../common/FileOperations.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../testExecution/TestMainWriter.h"
#include "../common/global/GlobalVariables.h"

typedef struct BuildSequenceStepInfo
{
    const char* description;
    const char* optionText;
    bool flagValue;
    void* function_ptr;
    const char* functionName;
} BuildSequenceStepInfo;

BuildSequenceStepInfo coreBuildSequenceInfo[9] = 
{
    {
        NULL_COMMAND_LINE_DESCRIPTION, 
        NULL_COMMAND_LINE_OPTION_TEXT, 
        NULL_COMMAND_LINE_FLAG_VALUE, 
        readConfigurationsFromFile, 
        "readConfigurationsFromFile"
    },
    {
        NULL_COMMAND_LINE_DESCRIPTION, 
        NULL_COMMAND_LINE_OPTION_TEXT, 
        NULL_COMMAND_LINE_FLAG_VALUE, 
        loadTestsAndSourceFiles, 
        "loadTestsAndSourceFiles"
    },
    {
        NULL_COMMAND_LINE_DESCRIPTION, 
        NULL_COMMAND_LINE_OPTION_TEXT, 
        NULL_COMMAND_LINE_FLAG_VALUE, 
        compileIntoTempObjectFiles, 
        "compileIntoTempObjectFiles"
    },
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        NO_TEST_FLAG_VALUE, 
        linkObjectFilesWithGregTestLibraryToMakeProjectTestLibrary, 
        "linkObjectFilesWithGregTestLibraryToMakeProjectTestLibrary"
    },
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        NO_TEST_FLAG_VALUE, 
        writeTestsToTestMain, 
        "writeTestsToTestMain"
    },
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        NO_TEST_FLAG_VALUE, 
        createTestMainExecutableFromProjectLibraryAndGregTestLibrary, 
        "createTestMainExecutableFromProjectLibraryAndGregTestLibrary"
    }, 
    {
        NO_TEST_DESCRIPTION, 
        NO_TEST_OPTION_TEXT, 
        NO_TEST_FLAG_VALUE, 
        runTestsWithExitStatusCheck, 
        "runTestsWithExitStatusCheck"
    },
    {
        NULL_COMMAND_LINE_DESCRIPTION, 
        NULL_COMMAND_LINE_OPTION_TEXT, 
        NULL_COMMAND_LINE_FLAG_VALUE, 
        compileIntoProjectExecutable, 
        "compileIntoProjectExecutable"
    },
    {
        DELETE_TEMP_DIR_DESCRIPTION, 
        DELETE_TEMP_DIR_OPTION_TEXT, 
        DELETE_TEMP_DIR_FLAG_VALUE, 
        removeTempDir, 
        "removeTempDir"
    }
};

#endif