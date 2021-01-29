#include "PluginLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/BuildSequenceStep.h"
#include "../common/GregBuildConstants.h"

typedef BuildSequenceStep *(*PluginFunction)();

void initPluginList(PluginList *list) {
  list->size = 0;
  list->plugins = (Plugin *)malloc(sizeof(Plugin));
  list->plugins[0].name = NULL;
}

void freePluginList(PluginList *list) {
  for (int i = 0; i < list->size; i++) {
    free(list->plugins[i].name);
  }
  free(list);
}

void freePluginHModules(LinkedList *pluginHModules) {
  for (int i = 0; i < pluginHModules->size; i++) {
    const HMODULE *hLib =
        (const HMODULE *)at_ll(pluginHModules, HMODULE_LL_TYPE, i);
    FreeLibrary(*hLib);
  }
  freeLinkedList(pluginHModules, &freeHModuleNode);
}

void freeHModuleNode(void *data) {}

void loadPlugins(PluginList *plugins, LinkedList *pluginHModules,
                 const char *basePath) {
  char *fileOrSubDirectoryFullPath =
      (char *)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char *));
  struct dirent *fileOrSubDirectory;

  DIR *basePathDirectory = opendir(basePath);
  if (!basePathDirectory) {
    return;
  }

  while ((fileOrSubDirectory = readdir(basePathDirectory)) != NULL) {
    copyNameIntoPath(fileOrSubDirectoryFullPath, basePath,
                     fileOrSubDirectory->d_name);
    addPluginToListOrContinueRecursion(plugins, pluginHModules, basePath,
                                       fileOrSubDirectory,
                                       fileOrSubDirectoryFullPath);
  }

  closedir(basePathDirectory);
  free(fileOrSubDirectoryFullPath);
}

void processPlugins(LinkedList *buildSequence, PluginList *list,
                    LinkedList *pluginHModules,
                    LinkedList *commandLineOptions) {
  for (int i = 0; i < list->size; i++) {
    const HMODULE *hLib =
        (const HMODULE *)at_ll(pluginHModules, HMODULE_LL_TYPE, i);
    PluginFunction beforeLoadingTestAndSourceFiles =
        (PluginFunction)GetProcAddress(*hLib,
                                       "beforeLoadingTestAndSourceFiles");
    BuildSequenceStep *beforeLoadingTestAndSourceFilesStep =
        beforeLoadingTestAndSourceFiles();
    insert_ll(buildSequence, beforeLoadingTestAndSourceFilesStep,
              BUILD_SEQUENCE_STEP_TYPE, 0);
    insert_ll(commandLineOptions, beforeLoadingTestAndSourceFilesStep->option,
              COMMAND_LINE_OPTION_TYPE, 0);
  }
}

void copyNameIntoPath(char *path, const char *basePath,
                      const char *fileOrSubDirectoryName) {
  strcpy(path, basePath);
  strcat(path, "/");
  strcat(path, fileOrSubDirectoryName);
}

void addPluginToListOrContinueRecursion(
    PluginList *plugins, LinkedList *pluginHModules, const char *basePath,
    const struct dirent *fileOrSubDirectory,
    const char *fileOrSubDirectoryFullPath) {
  if (isPlugin(fileOrSubDirectory)) {
    addPluginToList(plugins, pluginHModules, fileOrSubDirectoryFullPath);
  } else if (isVisibleDirectory(fileOrSubDirectory)) {
    loadPlugins(plugins, pluginHModules, fileOrSubDirectoryFullPath);
  }
}

bool isPlugin(const struct dirent *fileOrSubDirectory) {
  char reversedLower[WINDOWS_MAX_PATH_LENGTH];
  reverseString(reversedLower, fileOrSubDirectory->d_name);
  bool result = (strncmp(reversedLower, "lld.", 4) == 0 &&
                 strstr(fileOrSubDirectory->d_name, "Plugin") != NULL);
  return result;
}

void addPluginToList(PluginList *list, LinkedList *pluginHModules,
                     const char *pluginPath) {
  if (list != NULL && pluginHModules != NULL) {
    list->plugins =
        (Plugin *)realloc(list->plugins, ((list->size + 1) * sizeof(Plugin)));
    list->plugins[list->size].name =
        (char *)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char *));
    strcpy(list->plugins[list->size].name, pluginPath);
    list->size++;

    HMODULE *hLib = (HMODULE *)malloc(sizeof(HMODULE));
    *hLib = LoadLibrary(list->plugins[list->size - 1].name);
    append_ll(pluginHModules, hLib, HMODULE_LL_TYPE);
  }
}

void printPluginInList(const PluginList *list) {
  if (list->size == 0) {
    printf("No Plugins Found\n");
  }
  for (int i = 0; i < list->size; i++) {
    printf("Plugins[%d]: %s\n", i, list->plugins[i].name);
  }
}