#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "PluginDefs.h"

#include <dirent.h>
#include <stdbool.h>

#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"

void initPluginList(PluginList *list);
void freePluginList(PluginList *list);
void freePluginHModules(LinkedList *pluginHModules);
void freeHModuleNode(void *data);
void loadPlugins(PluginList *plugins, LinkedList *pluginHModules,
                 const char *basePath);
void copyNameIntoPath(char *path, const char *basePath,
                      const char *fileOrSubDirectoryName);
void addPluginToListOrContinueRecursion(PluginList *plugins,
                                        LinkedList *pluginHModules,
                                        const char *basePath,
                                        const struct dirent *fileOrSubDirectory,
                                        const char *fileOrSubDirectoryFullPath);
bool isPlugin(const struct dirent *fileOrSubDirectory);
void addPluginToList(PluginList *list, LinkedList *pluginHModules,
                     const char *pluginPath);
void printPluginInList(const PluginList *list);

#endif