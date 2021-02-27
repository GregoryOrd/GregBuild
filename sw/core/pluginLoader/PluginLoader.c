#include "PluginLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __WINDOWS__
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions_ll.h"
#include "../../external/GregCToolkit/sw/FileSystem/FileIO/FileReader.h"
#include "../../external/GregCToolkit/sw/FileSystem/FileSystemRecurser.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"

//////////////////////////////////////////////////////////////////////
//              Private Data and Function Prototypes                //
//////////////////////////////////////////////////////////////////////
bool addIfIsPlugin(ArgList* argList, const struct dirent* fileOrSubDirectory, const char* pluginPath);
void addPluginsNotListedInTheOrderConfigFileToTheEndOfTheTempLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginModules, LinkedList* tempPluginModules);
void addPluginToList(PluginList* list, LinkedList* pluginModules, const char* pluginPath);
void copyTempListsIntoActualLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginModules, LinkedList* tempPluginModules);
void freeModuleNode(void* data);
void initPluginList(PluginList* list);
bool isPlugin(const struct dirent* fileOrSubDirectory);
void orderPluginsToMatchConfigFile(PluginList* list, LinkedList* pluginModules);
int processOrderConfigEntry(ArgList* argsList);
int readPluginsFromOrderConfigFileIntoTempLists(const char* pathToTestFile, PluginList* list, PluginList* tempPluginList, LinkedList* tempPluginModules);

//////////////////////////////////////////////////////////////////////
//                     Function Implementations                     //
//////////////////////////////////////////////////////////////////////

void initPluginList(PluginList* list)
{
   list->size = 0;
   list->plugins = malloc(sizeof(Plugin));
   list->plugins[0].name = NULL;
}

void freePluginList(PluginList* list)
{
   for (int i = 0; i < list->size; i++)
   {
      free(list->plugins[i].name);
   }
   free(list->plugins);
   free(list);
}

void freePluginModules(LinkedList* pluginModules)
{
#ifdef __WINDOWS__
   for (int i = 0; i < pluginModules->size; i++)
   {
      const HMODULE* lib = (const HMODULE*)at_ll(pluginModules, PLUGIN_MODULE_LL_TYPE, i);
      FreeLibrary(*lib);
   }
   free(pluginModules, freeModuleNode);
#else
   for (int i = 0; i < pluginModules->size; i++)
   {
      void* lib = (void*)at_ll(pluginModules, PLUGIN_MODULE_LL_TYPE, i);
      dlclose(lib);
   }
   freeLinkedList(pluginModules, freeModuleNode);
#endif
}

void freeModuleNode(void* data) {}

void loadPlugins(PluginList* plugins, LinkedList* pluginModules, const char* basePath)
{
   initPluginList(plugins);
   initEmptyLinkedList(pluginModules, PLUGIN_MODULE_LL_TYPE);
   ArgList* argList = malloc(sizeof(ArgList));
   argList->size = 2;
   argList->args = calloc(2, sizeof(void*));
   argList->args[0] = plugins;
   argList->args[1] = pluginModules;
   recurseAndAddFilesToList(basePath, addIfIsPlugin, argList);
   freeArgList(argList);
   orderPluginsToMatchConfigFile(plugins, pluginModules);
}

bool isPlugin(const struct dirent* fileOrSubDirectory)
{
   char reversedLower[WINDOWS_MAX_PATH_LENGTH];
   reverseString(reversedLower, fileOrSubDirectory->d_name);
   bool result = (strncmp(reversedLower, REVERSED_LIBRARY_EXTENSION, LIBRARY_EXTENSION_LENGTH) == 0 && strstr(fileOrSubDirectory->d_name, "Plugin") != NULL);
   return result;
}

bool addIfIsPlugin(ArgList* argList, const struct dirent* fileOrSubDirectory, const char* pluginPath)
{
   if (!isPlugin(fileOrSubDirectory))
   {
      return false;
   }

   PluginList* list = (PluginList*)argList->args[0];
   LinkedList* pluginModules = (LinkedList*)argList->args[1];

   addPluginToList(list, pluginModules, pluginPath);
   return true;
}

void addPluginToList(PluginList* list, LinkedList* pluginModules, const char* pluginPath)
{
   if (list != NULL && pluginModules != NULL)
   {
      list->plugins = (Plugin*)realloc(list->plugins, ((list->size + 1) * sizeof(Plugin)));
      list->plugins[list->size].name = calloc(strlen(pluginPath) + 1, sizeof(char));
      strcpy(list->plugins[list->size].name, pluginPath);
      list->size++;

#ifdef __WINDOWS__
      HMODULE* lib = malloc(sizeof(HMODULE));
      *lib = LoadLibrary(list->plugins[list->size - 1].name);
      append_ll(pluginModules, lib, PLUGIN_MODULE_LL_TYPE);
#else
      void* lib = malloc(sizeof(void*));
      lib = dlopen(list->plugins[list->size - 1].name, RTLD_LAZY);
      append_ll(pluginModules, lib, PLUGIN_MODULE_LL_TYPE);
#endif
   }
}

void orderPluginsToMatchConfigFile(PluginList* list, LinkedList* pluginModules)
{
   PluginList* tempPluginList = malloc(sizeof(PluginList));
   initPluginList(tempPluginList);
   LinkedList* tempPluginModules = malloc(sizeof(LinkedList));
   initEmptyLinkedList(tempPluginModules, PLUGIN_MODULE_LL_TYPE);

   int error = readPluginsFromOrderConfigFileIntoTempLists(PLUGINS_LOAD_ORDER_CONFIG_FILE, list, tempPluginList, tempPluginModules);
   if (!error)
   {
      addPluginsNotListedInTheOrderConfigFileToTheEndOfTheTempLists(list, tempPluginList, pluginModules, tempPluginModules);
      copyTempListsIntoActualLists(list, tempPluginList, pluginModules, tempPluginModules);
   }

   freePluginList(tempPluginList);
   freeModuleNode(tempPluginModules);
}

int readPluginsFromOrderConfigFileIntoTempLists(const char* pathToTestFile, PluginList* list, PluginList* tempPluginList, LinkedList* tempPluginModules)
{
   ArgList* argsList = malloc(sizeof(ArgList));
   argsList->size = 3;
   argsList->args = calloc(argsList->size, sizeof(char*));
   argsList->args[0] = (void*)list;
   argsList->args[1] = (void*)tempPluginList;
   argsList->args[2] = (void*)tempPluginModules;

   int result = readFileWithActionAfterEachLine(pathToTestFile, argsList, processOrderConfigEntry);
   freeArgList(argsList);
   return result;
}

int processOrderConfigEntry(ArgList* argsList)
{
   PluginList* list = (PluginList*)argsList->args[0];
   PluginList* tempPluginList = (PluginList*)argsList->args[1];
   LinkedList* tempPluginModules = (LinkedList*)argsList->args[2];
   char* buffer = (char*)argsList->args[argsList->size - 1];

   for (int i = 0; i < list->size; i++)
   {
      if (strstr(list->plugins[i].name, buffer))
      {
         char pluginPath[WINDOWS_MAX_PATH_LENGTH] = PLUGINS_LIB_DIRECTORY;
         strcat(pluginPath, DELIMITER);
         strcat(pluginPath, buffer);
         strcat(pluginPath, LIBRARY_EXTENSION);
         addPluginToList(tempPluginList, tempPluginModules, pluginPath);
      }
   }
   return 0;
}

void addPluginsNotListedInTheOrderConfigFileToTheEndOfTheTempLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginModules, LinkedList* tempPluginModules)
{
   for (int i = 0; i < list->size; i++)
   {
      bool wasFoundInOrderConfigFile = false;
      for (int j = 0; j < tempPluginList->size; j++)
      {
         if (strcmp(list->plugins[i].name, tempPluginList->plugins[j].name) == 0)
         {
            wasFoundInOrderConfigFile = true;
         }
      }
      if (!wasFoundInOrderConfigFile)
      {
         addPluginToList(tempPluginList, tempPluginModules, list->plugins[i].name);
      }
   }
}

void copyTempListsIntoActualLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginModules, LinkedList* tempPluginModules)
{
   for (int i = 0; i < tempPluginList->size; i++)
   {
      strcpy(list->plugins[i].name, tempPluginList->plugins[i].name);
#ifdef __WINDOWS__
      setAt_ll(pluginModules, (HMODULE*)at_ll(tempPluginModules, PLUGIN_MODULE_LL_TYPE, i), PLUGIN_MODULE_LL_TYPE, i);
#else
      setAt_ll(pluginModules, (void*)at_ll(tempPluginModules, PLUGIN_MODULE_LL_TYPE, i), PLUGIN_MODULE_LL_TYPE, i);
#endif
   }
}

void printPluginInList(const PluginList* list)
{
   if (list->size == 0)
   {
      printf("No Plugins Found\n");
   }
   for (int i = 0; i < list->size; i++)
   {
      printf("Plugins[%d]: %s\n", i, list->plugins[i].name);
   }
}