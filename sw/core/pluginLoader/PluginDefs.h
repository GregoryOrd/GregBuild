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
#else
#define LIBRARY_EXTENSION ".so"
#endif

#endif