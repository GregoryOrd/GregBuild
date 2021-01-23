# Greg Build

The GregBuild works by using the fileSystemRecursion module to search through the file system recursively to find test and source files. Once all of the source files, test files, and test cases are identified, GregBuild uses the testMainWriting module to write the temp/TestMain.c and temp/TestMain.h files. The temp/TestMain.c file holds a function pointer for each test case that was identified in the repo and eventually make calls to those function pointers to execute each test case. 

After writing the temp/TestMain.c and temp/testMain.h files, GregBuild compiles all of the found source and test files into object (.o) files and then links them into the temp/TestProject.dll library. The TestMain.c file is then compiled and linked with the TestProject.dll and GregTest.dll libraries to create the temp/TestMain.exe executable.

Once GregBuild creates this TestMain executable, it runs the executable. Pass and fail messages for each test case is handled by the functions inside GregTest.dll and the TestMain executable exit status indicates if the tests passed or failed. If this exit status indicates all tests passed, GregBuild links the .o files (in the "temp" directory) that came from source files into
the final project executable. It places this executable inside the "dist" directory, then deletes the "temp" directory.

## Configurability

Currently GregBuild does not support any configurability and can only execute with default settings. Some of these defauly settings are defined in core/GregBuildConstants.h. In the future these values could be made resource or potientially command line configuable. Some of the rules for what defines a test (as found in the fileSystemRecursion module could also be made resource configurable.

GregBuild operates one level of abstraction higher than the actual compiler program. So thinking of the entire system as a layered architecture, it would be possible for the compiler used at the level below GregBuild could also be swapped out. This is also something that could be added as a configurable feature in the future.

Finally, some command line flags such a "--no-test-build" or something similar could be valuable feature to be had in the future for configuaring the behaviour of the build.

## sw/core
This directory holds the core module that make up GregBuild. Underneath this we have the "main" module, "common" module, and variout other modules. One of the goals of the architecture here is to setup the dependencies correctly so that each module only depends on the toolkit and some shared headers in the common directory. The "main" module can then depend on the other modules. This way each module can be compiled independtly before the "main" module is compiled with all of the other modules.

## external
This directory holds to git submodules, GregTest and GregCToolkit.

## plugins
This directory will hold plugins.