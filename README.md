# GregBuild

A simple build system for C projects. Uses [GregTest](https://github.com/GregoryOrd/GregTest) to execute unit tests during the build process. At the moment it is very simplistic and does not have many features have different configurations or extensibility, although some of this may be coming later. See the "Configurability" section of [Architecture.md](sw/Architecture.md) for more details on what future features may be coming to expand the usability of GregBuild for more use cases.

## Platform

This build system is being developed on a Windows machine using gcc and Cygwin. Some code has tried to use things such a pre-processor directives to provide cross-platform support. However, this is untested and may stil need some tweaking to get working. 