# Global Variables

The "interface" for the other modules in the program to use the global variables is
GlobalVariables.h. Behind this is ConfigurableValues and NonConfigurableValues.
For this reason, ConfigurableValues.h and NonConfigurableValues.h hold the data members
but do not provide any function declarations. Rather, the accessor methods for the data 
members are in GlobalVariables.h and any function needed for the logic are declared
as function prototypes at the top of the .c files.