# Greg Build

The GregBuild works by using the fileSystemRecursion module to search through the file system recursively to find test and source files. Once all of the source files, test files, and test cases are identified, GregBuild uses the testMainWriting module to write the temp/TestMain.c and temp/TestMain.h files. The temp/TestMain.c file holds a function pointer for each test case that was identified in the repo and eventually make calls to those function pointers to execute each test case. 

After writing the temp/TestMain.c and temp/testMain.h files, GregBuild compiles all of the found source and test files into object (.o) files and then links them into the temp/TestProject.dll library. The TestMain.c file is then compiled and linked with the TestProject.dll and GregTest.dll libraries to create the temp/TestMain.exe executable.

Once GregBuild creates this TestMain executable, it runs the executable. Pass and fail messages for each test case is handled by the functions inside GregTest.dll and the TestMain executable exit status indicates if the tests passed or failed. If this exit status indicates all tests passed, GregBuild links the .o files (in the "temp" directory) that came from source files into
the final project executable. It places this executable inside the "dist" directory, then deletes the "temp" directory.

## Configurability

Currently GregBuild does not support any configurability and can only execute with default settings. Some of these defauly settings are defined in core/GregBuildConstants.h. In the future these values could be made resource or potientially command line configuable. Some of the rules for what defines a test (as found in the fileSystemRecursion module could also be made resource configurable.

GregBuild operates one level of abstraction higher than the actual compiler program. So thinking of the entire system as a layered architecture, it would be possible for the compiler used at the level below GregBuild could also be swapped out. This is also something that could be added as a configurable feature in the future.

Finally, some command line flags such a "--no-test-build" or something similar could be valuable feature to be had in the future for configuaring the behaviour of the build.

## fileSystemRecursion Module

This module contains the domain models for source files, object (.o) files, test files, and test cases. These are found in the following header files:
	- SourceFileStructureDefs.h
	- ObjectFileStructureDefs.h
	- TestStructureDefs.h
	
The definitions for what differentiates a test file from a source file, and what format a test case function must have are found in TestAndSrcDefintions.

## testMainWriting Module

This module writes the TestMain.c and TestMain.h files that are eventually compiled into the TestMain executable to run the tests.

## externalProgramExecution

To accomplish the compiling and linking tasks that it needs, GregTest uses the gcc compiler. Some other common command line programs such as cp, mv, and mkdir are also used by GregTest. This module handles forking processes and making these calls.