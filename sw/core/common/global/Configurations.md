# Adding a Resource File Configurable Variable

To add a resource file configurable variable, add to a data member
in ConfigurableValues.h. Then add to compilerConfigParams in ConfigurableValues.c to specify the parameter used in the config file to mark the variable. In here, also specify the action function pointer and data if possible, telling the system how to save the value from the file into the data member. Also increment NUM_COMPILER_CONFIG_PARAMS.

If the value is being save to a singular data member, see the string_copy action function
as an example. If the value is being added to a list, you will need to tell the system
which list to add the value to in the setDataToActOn() function. See the things already
in that function for examples. Make sure to line up the indexing with variable 'i'
correctly with respect to the 'configurations' list. 

For a linkedlist you will also need to #define a type integer for the linked list in GregBuildConstants.h, and added a case for the list type in the listTypeFromData() function

Finally, add a free call of freeing function and an init function for the new data member. The free and init functions can go in ConfigurableValues.c. The init function needs to be called at 
the start of readConfigurationsFromFile(). The free function should be called at the end of the main function in GregBuildMain.c.

If a getter function is needed for the new data member, or the free function needs to be
available from other .c files, define these functions in GlobalVariables.h. The init function
can be defined privately as a function prototype at the top of the ConfigurableValues.c file.