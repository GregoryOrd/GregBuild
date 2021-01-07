# Plugin Extensions

To make this extensible, the idea exists to support a plugin architecture. The current idea is that plugins will have the ablity to add a command line options. They will also be able to provide function pointers that can be inserted in the the sequence, and an index for where that should be inserted into the sequence.

Each plugin will be able to provide multiple command line options and multiple function pointers. For example, a total time tracking plugin would need a function pointer at the beginning of the sequence to mark the start time and a second function pointer at the end of the sequence to mark the finishing time and calculate the elapsed time.

We will need to have some logic to decide what are valid insertions and how to handle the case that two plugins request to insert a step at the same index. Perhaps we could have some other command line options to allow the user to make this decision.

We should also consider if a plugin would be allowed to remove a command line option or step from the sequence. All of this will need to be built into an API, and we will need to create some plugin developer documentation describing what is available to the plugin developer and what is expected of their plugin.

With this, we will also need to write a plugin loader.

## Function Pointer Rules
The function pointer must return an integer exit status, 1 for failure and 0 for success.