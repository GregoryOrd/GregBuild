#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "PluginDefs.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "../../external/GregCToolkit/sw/ArgList/ArgList.h"

#ifdef __cplusplus
extern "C" {
#endif  

void freePluginList(PluginList *list);
void freePluginModules(LinkedList *pluginModules);
void loadPlugins(PluginList *plugins, LinkedList *pluginModules,
                 const char *basePath);
void printPluginInList(const PluginList *list);

#ifdef __cplusplus
}
#endif  

#endif