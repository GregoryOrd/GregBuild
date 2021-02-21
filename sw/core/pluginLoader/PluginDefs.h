#ifndef PLUGIN_DEFS_H
#define PLUGIN_DEFS_H

typedef struct Plugin { char *name; } Plugin;

typedef struct PluginList {
  int size;
  Plugin *plugins;
} PluginList;

#define DELIMITER "/"

#ifdef __WINDOWS__
#define LIBRARY_EXTENSION ".dll"
#define REVERSED_LIBRARY_EXTENSION "lld."
#define LIBRARY_EXTENSION_LENGTH 4
#else
#define LIBRARY_EXTENSION ".so"
#define REVERSED_LIBRARY_EXTENSION "os."
#define LIBRARY_EXTENSION_LENGTH 3
#endif

#endif