# Adding a Resource File Configurable Variable

To add a resource file configurable variable, add to the configurations member
in CompilerConfiguration.c to specify the parameter used in the config file to mark
this variable. In here, also specify the action function pointer and data if possible,
telling the system how to save the value from the file into the data member. Also 
increment NUM_COMPILER_CONFIG_PARAMS.

If the value is being save to a singular data member, see the string_copy action function
as an example. If the value is being added to a list, you will need to tell the system
which list to add the value to in the setDataToActOn() function. See the things already
in that function for examples. Make sure to line up the indexing with variable 'i'
correctly with respect to the 'configurations' list.