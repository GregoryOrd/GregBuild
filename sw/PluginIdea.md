# Plugin Extensions

To make this extensible, the idea exists to support a plugin architecture. The current idea is that plugins will have the ablity to add a command line option. They will also be able to provide a function pointer that can be inserted in the the sequence, and an index for where that should be inserted into the sequence.

We will need to have some logic to decide what are valid insertions and how to handle the case that two plugins request to insert a step at the same index. Perhaps we could have some other command line options to allow the user to make this decision.

We should also consider if a plugin would be allowed to remove a command line option or step from the sequence. All of this will need to be built into an API, and we will need to create some plugin developer documentation describing what is available to the plugin developer and what is expected of their plugin.

With this, we will also need to write a plugin loader.