#include "ExternalProgramExecution.h"

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* for pid_t */

#ifdef __unix__
#include <unistd.h> /* for fork */
#include <sys/wait.h> /* for wait */
#endif

#ifdef __WIN32__
#include <process.h>
#endif

int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[])
{
    int status;
    #ifdef __unix__
    /*Spawn a child to run the program.*/
    pid_t pid=fork();
    if (pid==0) { /* child process */
        execv(pathToExecutable, argv);
        exit(1); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        int status;
        waitpid(pid, &status, 0); /* wait for child to exit */
        if (WIFEXITED(status)) 
        { 
            return WEXITSTATUS(status);    
        } 
    } 
    #endif

    #ifdef __WIN32__
    status = spawnv(P_WAIT, pathToExecutable, argv);
    if(WIFEXITED(status)) 
    { 
        return WEXITSTATUS(status);   
    } 
    #endif
}