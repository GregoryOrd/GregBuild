# GregBuild

A simple build system for C projects. Uses [GregTest](https://github.com/GregoryOrd/GregTest) to execute unit tests during the build process. At the moment it is very simplistic and does not have many features have different configurations or extensibility, although some of this may be coming later. See the "Configurability" section of [Architecture.md](sw/Architecture.md) for more details on what future features may be coming to expand the usability of GregBuild for more use cases.

## Platform

This build system is being developed on a Windows machine using gcc and Cygwin. Some code has tried to use things such a pre-processor directives to provide cross-platform support. However, this is untested and may stil need some tweaking to get working. 

## Compilers

Different compilers can be used by GregBuild by adding a compiler.config file inside of your repository. See [CompilerConfiguration.md](CompilerConfiguration.md) for me details. Please ensure the compiler and all of its depencies are in a location that
can be reached by GregBuild. For example, if running GregBuild inside of Cygwin,
please make sure the compiler is also located within Cygwin. It may vary based on where Cygwin is installed. But using my path as an example, the compiler files should be located somewhere under "C:/cygwin64"

## Adding dirent to MSVC include path

In this project dirent.h is used to recurse through the file system. At the time of writing the functions for this recursion I was developing with VS Code and compiling with gcc and Cygwin. By default the VS CODE and the MSVC compiler do not include dirent.h, but Cygwin does. This meant that I would get a file not found error inside of my IDE, but would not have the problem when compiling. To remove this error inside the IDE, I added dirent.h to the MSVC include path. In my case, the include path was

- C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/include

The dirent.h file that I placed here came from a github repo owned by Toni Ronkko:
- [https://github.com/tronkko/dirent/blob/master/include/dirent.h](https://github.com/tronkko/dirent/blob/master/include/dirent.h)

## Adding ATmega 328p Files to MSVC include path and VS Code Configuration

The "Blink" example repo used here is being used for testing the GregBuild can cross-compile to a different target hardware.
For this testing, I am using a development board with an Atmega328p. To support this,
the includes for the avr-gcc compiler were also copied into the MSVC include path.

After adding to the MSVC include path, there are still issues when <avr/io.h> is included. This header looks for the defined microcontroller (Atmega328p) and then includes <avr/iom328p.h>. However, VS code doesn't know what microcontroller to use, so it has issues with this. To remove the VS code errors, add "__AVR_ATmega328P__" to the defines list inside .vscode/c_cpp_properties.json.