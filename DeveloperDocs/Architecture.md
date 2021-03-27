# Greg Build Architecture

## Note

Some markdown files are included beside the code in the repo's file structure. 
These will contains more specific details about the area of the code where
they are found. The information in this Architecture.md file is a higher level
overview of how GregBuild works.

## GregBuild

The GregBuild works by using the fileSystemRecursion module to search through the file system recursively to find test and source files. Once all of the source files, test files, and test cases are identified, GregBuild uses the testMainWriting module to write the temp/TestMain.c and temp/TestMain.h files. The temp/TestMain.c file holds a function pointer for each test case that was identified in the repo and eventually make calls to those function pointers to execute each test case. 

After writing the temp/TestMain.c and temp/testMain.h files, GregBuild compiles all of the found source and test files into object (.o) files and then links them into the temp/TestProject.dll/TestProject.so library. The TestMain.c file is then compiled and linked with the TestProject.dll/.so and GregTest.dll/.so libraries to create the temp/TestMain.exe executable.

Once GregBuild creates this TestMain executable, it runs the executable. Pass and fail messages for each test case is handled by the functions inside GregTest.dll/.so and the TestMain executable exit status indicates if the tests passed or failed. If this exit status indicates all tests passed, GregBuild links the .o files (in the "temp" directory) that came from source files into
the final project executable. It places this executable inside the "dist" directory, then deletes the "temp" directory.

## sw/core
This directory holds the core module that make up GregBuild. Underneath this we have the "main" module, "common" module, and various other modules. One of the goals of the architecture here is to setup the dependencies correctly so that each module only depends on the toolkit and some shared headers in the common directory. The "main" module can then depend on the other modules. This way each module can be compiled independtly before the "main" module is compiled with all of the other modules.

## external
This directory holds to git submodules, GregTest and GregCToolkit.

## plugins
This directory holds plugins.