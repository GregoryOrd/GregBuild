# Plugin Extensions

To make this extensible, the idea exists to support a plugin architecture. The current idea is that plugins will have the ablity to add a command line options. They will also be able to provide function pointers that can be inserted in the the sequence, and an index for where that should be inserted into the sequence.

Each plugin will be able to provide multiple command line options and multiple function pointers. For example, a total time tracking plugin would need a function pointer at the beginning of the sequence to mark the start time and a second function pointer at the end of the sequence to mark the finishing time and calculate the elapsed time.

We will need to have some logic to decide what are valid insertions and how to handle the case that two plugins request to insert a step at the same index. Perhaps we could have some other command line options to allow the user to make this decision.

We should also consider if a plugin would be allowed to remove a command line option or step from the sequence. All of this will need to be built into an API, and we will need to create some plugin developer documentation describing what is available to the plugin developer and what is expected of their plugin.

With this, we will also need to write a plugin loader.

## Plugin Categories?
Managing multiple plugins. Maybe we can have plugin "categories". Each plugin must register itself with
a specific category. The categories can be designed such that a plugin from one category will not overlap, 
interfere, or conflict with a plugin from another category. From there, the plugin loader selects only a single plugin from that category.

Some thoughts about this idea:
    - Does this break the idea that the plugin knows about the core, but the core doesn't know about the plugin?
    - If we have this set of "catergories", then different plugins within the same category are really 
    just different implementations of the same functionality that is predefined by the categories.
        - This means that the plugin isn't really extending the core functionality, it is just switching
        out the implmentation of part of the core's functionality.
        - Maybe this flexibility is good? Or maybe we should just implement these core functionalities once
        in the core of the application.

## Function Pointer Rules
The function pointer must return an integer exit status, 1 for failure and 0 for success.