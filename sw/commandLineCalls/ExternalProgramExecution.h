#ifndef EXTERNAL_PROGRAM_EXECUTION_H
#define EXTERNAL_PROGRAM_EXECUTION_H

int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[]);

#endif