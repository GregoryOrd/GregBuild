#include "PluginLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/BuildSequenceStep.h"
#include "../common/FileStructureDefs.h" //Do we need this include?
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

void loadPlugins(PluginList *plugins, const char *basePath) {
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
    addPluginToListOrContinueRecursion(plugins, basePath, fileOrSubDirectory,
                                       fileOrSubDirectoryFullPath);
  }

  closedir(basePathDirectory);
  free(fileOrSubDirectoryFullPath);
}

void processPlugins(LinkedList *buildSequence, PluginList *list) {
  for (int i = 0; i < list->size; i++) {
    // Need to maintain a list of open libraries in GregBuildMain
    // so that we can free those libraries at the end of the program.
    // If we open and close the libraries in here, then we can't
    // run the appropriate functions as part of the build sequence.
    HMODULE hLib = LoadLibrary(list->plugins[i].name);
    PluginFunction beforeLoadingTestAndSourceFiles =
        (PluginFunction)GetProcAddress(hLib, "beforeLoadingTestAndSourceFiles");
    BuildSequenceStep *beforeLoadingTestAndSourceFilesStep =
        beforeLoadingTestAndSourceFiles();
    insert_ll(buildSequence, beforeLoadingTestAndSourceFilesStep,
              BUILD_SEQUENCE_STEP_TYPE, 0);
    // also need to insert the command line option into the command line options
    // list
    // FreeLibrary(hLib);
  }
}

void copyNameIntoPath(char *path, const char *basePath,
                      const char *fileOrSubDirectoryName) {
  strcpy(path, basePath);
  strcat(path, "/");
  strcat(path, fileOrSubDirectoryName);
}

void addPluginToListOrContinueRecursion(
    PluginList *plugins, const char *basePath,
    const struct dirent *fileOrSubDirectory,
    const char *fileOrSubDirectoryFullPath) {
  if (isPlugin(fileOrSubDirectory)) {
    addPluginToList(plugins, fileOrSubDirectoryFullPath);
  } else if (isVisibleDirectory(fileOrSubDirectory)) {
    loadPlugins(plugins, fileOrSubDirectoryFullPath);
  }
}

bool isPlugin(const struct dirent *fileOrSubDirectory) {
  char reversedLower[WINDOWS_MAX_PATH_LENGTH];
  reverseString(reversedLower, fileOrSubDirectory->d_name);
  bool result = (strncmp(reversedLower, "lld.", 4) == 0 &&
                 strstr(fileOrSubDirectory->d_name, "Plugin") != NULL);
  return result;
}

void addPluginToList(PluginList *list, const char *pluginPath) {
  if (list != NULL) {
    list->plugins =
        (Plugin *)realloc(list->plugins, ((list->size + 1) * sizeof(Plugin)));
    list->plugins[list->size].name =
        (char *)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char *));
    strcpy(list->plugins[list->size].name, pluginPath);
    list->size++;
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