#include "PluginLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileStructureDefs.h"

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