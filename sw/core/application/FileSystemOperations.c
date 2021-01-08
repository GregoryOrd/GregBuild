#include "FileSystemOperations.h"

#include "GregBuildConstants.h"
#include "../externalProgramExecution/CommandLineExecutables.h"
#include "../externalProgramExecution/ExternalProgramExecution.h"

#include <stdlib.h>

void makeDir(char* dirName)
{
    char* const argv[] = {mkdir, dirName, NULL};
    forkAndRunChildProcess(mkdir, argv);
}

void removeDir(char* folderName)
{
    char * const argv[] = {rm, folderName, "-r", NULL};
    forkAndRunChildProcess(rm, argv);  
}

void copyDllsToCurrentDirectory()
{
    char * const argv[] = {cp, TEMP_TEST_PROJECT_DLL, CURRENT_DIR, NULL};
    forkAndRunChildProcess(cp, argv);

    char * const argv1[] = {cp, LIB_GREG_TEST_DLL, CURRENT_DIR, NULL};
    forkAndRunChildProcess(cp, argv1);
}

void removeDllsFromCurrentDirectory()
{
    char * const argv[] = {rm, GREG_TEST_DLL, NULL};
    forkAndRunChildProcess(rm, argv); 

    char * const argv1[] = {rm, TEST_PROJECT_DLL, NULL};
    forkAndRunChildProcess(rm, argv1);
}