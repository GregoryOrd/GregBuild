# GregBuild Configuration

## Configuration File

The file specifying the settings you would like to configure for your project
should be named "config" with no file extension and should be placed at the root
of your repository.

## List of Configurable Settings

Below is a list of the settings that are configurable from the "config" file.
The name column is the parameter text that goes in the "config" file.


| Name   |      Description      |
|----------|:-------------:|
| host |  The compiler to be used as the "host" compiler |
| target |    The compiler to be used as the "target" compiler   |
| executableName | The file name of the executable file produced by GregBuild|
| compilerOption | Adds to a list of options used by both the host and target compilers when compiling to .o files|
| hostCompilerOption | Adds to a list of options used by the host compiler when compiling to .o files|
| targetCompilerOption | Adds to a list of options used by the target compiler when compiling to .o files|
| linkerOption | Adds to a list of options used by both the host and target compilers when linking |
| hostLinkerOption | Adds to a list of options used by the host compiler when linking |
| targetLinkerOption | Adds to a list of options used by the target compiler when linking |
| hostExcludedFile | Adds to a list of files to be excluded from the compilation with the host compiler. This may be hardware specific files for example.|
| targetExcludedFile | Adds to a list of files to be excluded from the compilation with the target compiler. This does not include test files which will always be excluded. An example of a file you might wish to exclude here could be a stub file that was only used as a placeholder in testing. |
| buildStartingDirectory | GregBuild should be run from the root of the repository so that it can find files such as the config file. However, once GregBuild starts looking for code, specifying this setting can tell GregBuild a subdirectory for where to start looking for the source code. Note that this setting is not nessecary to set, but can be an optimization so that GregBuild doesn't waste time looking through subdirectories that don't contain any source code. |
| testFrameworkLibrary | The path to the test framework library that you want GregBuild to use. This will default to /usr/lib/GregTest.so |


## Telling GregBuild What Compiler To Use

To configure the compilers used by GregBuild, add or use a file called "config" in
the root directory of the repo to be built with GregBuild. Add a line to specify the
host compiler and a line to specify the target compiler. For example:

```
host:/usr/bin/gcc.exe
target:/opt/avr8-gnu-toolchain/bin/avr-gcc.exe
```

If there are issues compiling with the host compiler, GregBuild will skip running the tests and proceed to attempting to compile with the host compiler. In the case that
the target compiler successfully builds the program, but the tests were not run on
the host, the final message will show as "Build Successful. NO TESTS WERE RUN".

If you want to build just for the host system, set the target compiler to match
the host compiler:

```
host:/usr/bin/gcc.exe
target:/usr/bin/gcc.exe
```

For GregBuild to use your compiler path, that compiler path needs to work with the environment outside of GregBuild. For example, inside of Cygwin the above samples
will work. However, trying to use "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe" may not work as Cygwin could give an error saying "command not found".


## Configuring Compiler Options

To configure compiler and linked options, add lines to the "config" file starting with 
`compilerOption:` or `linkerOption:`. For example:

```
compilerOption:-DF_CPU=16000000UL
compilerOption:-I/opt/atmel/ATmega_DFP/1.6.364/include
compilerOption:-mmcu=atmega328p
compilerOption:-Og

linkerOption:-Wl,-Map="blink.map"
linkerOption:-Wl,--start-group
linkerOption:-Wl,-Wl,-lm
linkerOption:-Wl,--end-group
```

The above entries will add compiler and linker options that are used when compiling
and linking with both the host and the target compilers. 

To add an option
for just the host compiler or linker, use `hostCompilerOption` or `hostLinkerOption`.
For just the target, use `targetCompilerOption ` or `targetLinkerOption`. For example:

```
hostCompilerOption:-D__AVR_ATmega328P__
hostCompilerOption:-D__DELAY_BACKWARD_COMPATIBLE__

hostLinkerOption:<Some Linker Option>

targetCompilerOption:-mmcu=atmega328p

targetLinkerOption:<Some Other Linker Option>
```