# Compiler Configuration


## Telling GregBuild What Compiler To Use

To configure the compilers used by GregBuild, place a file called "compiler.config" in
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
host=/usr/bin/gcc.exe
target=/usr/bin/gcc.exe
```

For GregBuild to use your compiler path, that compiler path needs to work with the environment outside of GregBuild. For example, inside of Cygwin the above samples
will work. However, trying to use "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe" may not work as Cygwin could give an error saying "command not found".


## Configuring Compiler Options

To configure compiler and linked options, add lines to compiler.config starting with 
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