# Command Line Options

## Structure of Command Line Options
Command line options are made up of three pieces of data:
- Option Text
- Description
- Flag Default Value

The option text is what the user type in the command line for the option. The description is a short piece of text displayed when printing the supported options. The flag default value is used to control how the option affects the build sequence.

Each build sequence step is tied to the null command line option or a non-null supported command line option. If the flag value is one at the time the build sequence is executed, then the build sequence step will be run. The flag value for a command line option is flipped if the user types the option text into the command line.

For example, the "--no-test-build" option is tied to the build sequence steps that build and run the tests. It has its flag default value set to one, meaning that it will run the tests by default. If this option is used the flag will be flipped to zero (when the command line options are processed, before the build sequence begins execution) and the test will not run.

## Adding a Command Line Option
Adding a core supported command line option is simple and is accomplished with the following steps:
- Increment NUM_SUPPORTED_COMMAND_LINE_OPTIONS in GregBuildConstants.h
- #define the option text, description, and flag default value in GregBuildConstants.h
- Add an element to the last with the option text, description, and flag default value in in the setCoreCommandLineOptions() function in CoreCommandLineOptions.c