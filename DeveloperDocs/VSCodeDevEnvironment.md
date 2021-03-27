# Setting up the Dev Environment in VS Code

## Adding dirent to MSVC include path on Windows

In this project dirent.h is used to recurse through the file system. If developing on
Windows using the Windows Subsystem for Linux, VS CODE and the MSVC compiler will not
include dirent.h. This means VS Code will give a file not found error, but when
compiling in the linux environmnet, there would be no problem. To remove this error 
inside the IDE, I added dirent.h to the MSVC include path. In my case, the include path was

- C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/include

The dirent.h file that I placed here came from a github repo owned by Toni Ronkko. It is important to note that this file does not actually get used in the software.
- [https://github.com/tronkko/dirent/blob/master/include/dirent.h](https://github.com/tronkko/dirent/blob/master/include/dirent.h)

## Adding ATmega 328p Files to MSVC include path and VS Code Configuration

The "Blink" example repo used here is being used for testing that GregBuild can 
cross-compile to a different target hardware. For this testing, I am using a
development board with an Atmega328p. To support this, the includes for the avr-gcc
compiler were also copied into the MSVC include path.

After adding to the MSVC include path, there are still issues when <avr/io.h> is included. This header looks for the defined microcontroller (Atmega328p) and then includes <avr/iom328p.h>. However, VS code doesn't know what microcontroller to use, so it has issues with this. To remove the VS code errors, add "__AVR_ATmega328P__" to the defines list inside .vscode/c_cpp_properties.json.