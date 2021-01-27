#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <dirent.h>
#include <stdbool.h>

typedef struct Plugin { char *name; } Plugin;

typedef struct PluginList {
  int size;
  Plugin *plugins;
} PluginList;

void initPluginList(PluginList *list);
void freePluginList(PluginList *list);
void loadPlugins(PluginList *plugins, const char *basePath);
void copyNameIntoPath(char *path, const char *basePath,
                      const char *fileOrSubDirectoryName);
void addPluginToListOrContinueRecursion(PluginList *plugins,
                                        const char *basePath,
                                        const struct dirent *fileOrSubDirectory,
                                        const char *fileOrSubDirectoryFullPath);
bool isPlugin(const struct dirent *fileOrSubDirectory);
void addPluginToList(PluginList *list, const char *pluginPath);
void printPluginInList(const PluginList *list);

#endif