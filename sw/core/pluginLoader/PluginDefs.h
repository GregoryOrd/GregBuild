#ifndef PLUGIN_DEFS_H
#define PLUGIN_DEFS_H

typedef struct Plugin { char *name; } Plugin;

typedef struct PluginList {
  int size;
  Plugin *plugins;
} PluginList;

#endif