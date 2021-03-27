# GregBuild

A build system for C/C++ projects. GregBuild can use any test framework that
is contained within a single .so library and any number of .h header files. By default
GregBuild will use [GregTest](https://github.com/GregoryOrd/GregTest) to execute unit 
tests during the build process. GregTest ships as a single .so and a single .h. The path
to the test framework library .so is configurable in the .config file.

## Platform

Currently GregBuild is only supported on linux platforms. However, Windows users, can
still use GregBuild on their machines if they run it inside of a Windows Subsystem for 
Linux environment. When examining file names for GregBuild and GregTest binaries, LX
refers to x86 and RP refers to Raspberry Pi Zero, where GregBuild has also been used.

## Cross-Compiling

A main feature of GregBuild is that is was designed to support cross-compiling.
GregBuild works in a interesting way such that it uses two compilers, each of which
can be specified by the user. The user can swap these compilers out using the config
file in the root of the repository. 

The first compiler is designated as the "host" compiler. This is the compiler for the 
hardware that GregBuild is running on. This compiler is used to compile code for 
execution in test on the host hardware.

The second compiler is designated as the "target" compiler. This compiler can be the 
same as the host or can be different. If it is different, the source code, excluding
tests, is re-compiled with this compiler after the tests are run to produce an
executable that will run on the target hardware.

For example, in the ExampleSrcRepo (included in the GregBuild repo as an example), the host and target compiler are the same. This means that GregBuild will compile the code on the host machine, run the tests on the host machine, and  then produce an executable
for the host machine. 

In the Blink example repo, the host and target are different. This means that GregBuild will compile the code and tests on the host machine, run the tests on the host machine, 
and then if the tests pass, compile again (excluding the tests) with the target 
compiler to produce an executable for the target hardware.

## Configuration

Many different things are configurable settings for GregBuild. To specify these settings
place a file name "config" with no extension at the root of the repository.

See [Configuration.md](CustomerDocs/Configuration.md) for more details on the available
configurations settings.
