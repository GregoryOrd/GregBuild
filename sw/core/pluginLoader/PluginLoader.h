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

void initPluginList(PluginList *list);
void freePluginList(PluginList *list);
void freePluginModules(LinkedList *pluginModules);
void freeModuleNode(void *data);
void loadPlugins(PluginList *plugins, LinkedList *pluginModules,
                 const char *basePath);
bool isPlugin(const struct dirent *fileOrSubDirectory);
bool addIfIsPlugin(ArgList* argList, const struct dirent* fileOrSubDirectory, const char* pluginPath);
void addPluginToList(PluginList* list, LinkedList* pluginModules, const char* pluginPath);
void orderPluginsToMatchConfigFile(PluginList* list, LinkedList* pluginModules);
int readPluginsFromOrderConfigFileIntoTempLists(const char* pathToTestFile, PluginList* list, PluginList* tempPluginList, LinkedList* tempPluginModules);
int processOrderConfigEntry(ArgList* argsList);
void addPluginsNotListedInTheOrderConfigFileToTheEndOfTheTempLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginModules, LinkedList* tempPluginModules);
void copyTempListsIntoActualLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginModules, LinkedList* tempPluginModules);
void printPluginInList(const PluginList *list);

#ifdef __cplusplus
}
#endif  

#endif