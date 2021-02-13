# Compiler Configuration


## Telling GregBuild What Compiler To Use

To configure the compilers used by GregBuild, place a file called "compiler.config" in
the root directory of the repo to be built with GregBuild. Add a line to specify the
host compiler and a line to specify the target compiler. For example:

```
host=/usr/bin/gcc.exe
target=/opt/avr8-gnu-toolchain/bin/avr-gcc.exe
```


If you want to build just for the host system, set the target compiler to match
the host compiler:

```
host=/usr/bin/gcc.exe
target=/usr/bin/gcc.exe
```

For GregBuild to use your compiler path, that compiler path needs to work with the environment outside of GregBuild. For example, inside of Cygwin the above samples
will work. However, trying to use "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe" may not work as Cygwin could give an error saying "command not found".


## Configuring Compiler Flags

Coming soon...