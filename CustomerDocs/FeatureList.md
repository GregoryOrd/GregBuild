# Feature List

- Cross-Compiling
- Supports adding your own plugins
    - A plugin can add a command line options so they can easily be unused
    without needing to remove a library from the plugins directory.
- No test build option
- Ability to exlcude files from the build to facilate
  stubing out target hardware specific files in tests
- See [Configuration.md](Configuration.md) for list of configurable settings

## Compiler Configurability

GregBuild operates one level of abstraction higher than the actual compiler program. So it is possible for the compiler(s) used at the level below GregBuild to be swapped out
easily. This is also something that can be configured in the repo's "config" file. See 
[Configuration.md](Configuration.md).


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

## Excluding Files from The Build

GregBuild supports running tests on the host platform while building for a different host platform. In some cases, these tests will need to have platform specific files excluded. To allow for this, GregBuild supports exlcuding files from the host and target builds. To exclude a file from the host build add a line to the config file with the parameter "hostExcludedFile". For example:

hostExcludedFile:src/blink/controlLEDs/drivers/LEDDriver.c

To exclude from the target build add a line with "targetExcludedFile". For example:

targetExcludedFile:src/blink/MyTargetFileToExclude.c

Note that GregBuild will always exclude compiling test files when compiling with the target compiler. In the case that the host and target compiler are the same, the test files will be compiled for the tests, but will not be linked into the final executable.

## Known Limitations

There is no dependency management baked into GregBuild. This means that the user
must organize their repos such that the dependencies are ordered alphabetically. The
alphabetical order is then used at link time to link everything. This is scheduled
to be improved in the 1.1 release.