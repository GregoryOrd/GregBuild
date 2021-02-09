#include "PluginLoader.h"

#include <stdio.h>
#include <windows.h>

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions_ll.h"
#include "../../external/GregCToolkit/sw/FileIO/FileReader.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"

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
   free(list);
}

void freePluginHModules(LinkedList* pluginHModules)
{
   for (int i = 0; i < pluginHModules->size; i++)
   {
      const HMODULE* hLib = (const HMODULE*)at_ll(pluginHModules, HMODULE_LL_TYPE, i);
      FreeLibrary(*hLib);
   }
   freeLinkedList(pluginHModules, &freeHModuleNode);
}

void freeHModuleNode(void* data) { free(data); }

void loadPlugins(PluginList* plugins, LinkedList* pluginHModules, const char* basePath)
{
   char fileOrSubDirectoryFullPath[WINDOWS_MAX_PATH_LENGTH] = "";
   struct dirent* fileOrSubDirectory;

   DIR* basePathDirectory = opendir(basePath);
   if (!basePathDirectory)
   {
      return;
   }

   while ((fileOrSubDirectory = readdir(basePathDirectory)) != NULL)
   {
      copyNameIntoPath(fileOrSubDirectoryFullPath, basePath, fileOrSubDirectory->d_name);
      addPluginToListOrContinueRecursion(plugins, pluginHModules, basePath, fileOrSubDirectory, fileOrSubDirectoryFullPath);
   }

   closedir(basePathDirectory);
}

void copyNameIntoPath(char* path, const char* basePath, const char* fileOrSubDirectoryName)
{
   strcpy(path, basePath);
   strcat(path, "/");
   strcat(path, fileOrSubDirectoryName);
}

void addPluginToListOrContinueRecursion(
    PluginList* plugins, LinkedList* pluginHModules, const char* basePath, const struct dirent* fileOrSubDirectory, const char* fileOrSubDirectoryFullPath)
{
   if (isPlugin(fileOrSubDirectory))
   {
      addPluginToList(plugins, pluginHModules, fileOrSubDirectoryFullPath);
   }
   else if (isVisibleDirectory(fileOrSubDirectory))
   {
      loadPlugins(plugins, pluginHModules, fileOrSubDirectoryFullPath);
   }
}

bool isPlugin(const struct dirent* fileOrSubDirectory)
{
   char reversedLower[WINDOWS_MAX_PATH_LENGTH];
   reverseString(reversedLower, fileOrSubDirectory->d_name);
   bool result = (strncmp(reversedLower, "lld.", 4) == 0 && strstr(fileOrSubDirectory->d_name, "Plugin") != NULL);
   return result;
}

void addPluginToList(PluginList* list, LinkedList* pluginHModules, const char* pluginPath)
{
   if (list != NULL && pluginHModules != NULL)
   {
      list->plugins = (Plugin*)realloc(list->plugins, ((list->size + 1) * sizeof(Plugin)));
      list->plugins[list->size].name = malloc(strlen(pluginPath));
      strcpy(list->plugins[list->size].name, pluginPath);
      list->size++;

      HMODULE* hLib = malloc(sizeof(HMODULE));
      *hLib = LoadLibrary(list->plugins[list->size - 1].name);
      append_ll(pluginHModules, hLib, HMODULE_LL_TYPE);
   }
}

void orderPluginsToMatchConfigFile(PluginList* list, LinkedList* pluginHModules)
{
   PluginList* tempPluginList = malloc(sizeof(PluginList));
   initPluginList(tempPluginList);
   LinkedList* tempPluginHModules = malloc(sizeof(LinkedList));
   initEmptyLinkedList(tempPluginHModules, HMODULE_LL_TYPE);

   readPluginsFromOrderConfigFileIntoTempLists(PLUGINS_LOAD_ORDER_CONFIG_FILE, list, tempPluginList, tempPluginHModules);
   addPluginsNotListedInTheOrderConfigFileToTheEndOfTheTempLists(list, tempPluginList, pluginHModules, tempPluginHModules);
   copyTempListsIntoActualLists(list, tempPluginList, pluginHModules, tempPluginHModules);

   freePluginList(tempPluginList);
   freeHModuleNode(tempPluginHModules);
}

void readPluginsFromOrderConfigFileIntoTempLists(const char* pathToTestFile, PluginList* list, PluginList* tempPluginList, LinkedList* tempPluginHModules)
{
   ArgList* argsList = malloc(sizeof(ArgList));
   argsList->size = 3;
   argsList->args = malloc(argsList->size * sizeof(char*));
   argsList->args[0] = (void*)list;
   argsList->args[1] = (void*)tempPluginList;
   argsList->args[2] = (void*)tempPluginHModules;

   readFileWithActionAfterEachLine(pathToTestFile, argsList, processOrderConfigEntry);
}

int processOrderConfigEntry(ArgList* argsList)
{
   PluginList* list = (PluginList*)argsList->args[0];
   PluginList* tempPluginList = (PluginList*)argsList->args[1];
   LinkedList* tempPluginHModules = (LinkedList*)argsList->args[2];
   char* buffer = (char*)argsList->args[argsList->size - 1];

   for (int i = 0; i < list->size; i++)
   {
      if (strstr(list->plugins[i].name, buffer))
      {
         char pluginPath[WINDOWS_MAX_PATH_LENGTH] = PLUGINS_LIB_DIRECTORY;
         strcat(pluginPath, "/");
         strcat(pluginPath, buffer);
         strcat(pluginPath, ".dll");
         addPluginToList(tempPluginList, tempPluginHModules, pluginPath);
      }
   }
   return 0;
}

void addPluginsNotListedInTheOrderConfigFileToTheEndOfTheTempLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginHModules, LinkedList* tempPluginHModules)
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
         addPluginToList(tempPluginList, tempPluginHModules, list->plugins[i].name);
      }
   }
}

void copyTempListsIntoActualLists(PluginList* list, PluginList* tempPluginList, LinkedList* pluginHModules, LinkedList* tempPluginHModules)
{
   for (int i = 0; i < tempPluginList->size; i++)
   {
      strcpy(list->plugins[i].name, tempPluginList->plugins[i].name);
      setAt_ll(pluginHModules, (HMODULE*)at_ll(tempPluginHModules, HMODULE_LL_TYPE, i), HMODULE_LL_TYPE, i);
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