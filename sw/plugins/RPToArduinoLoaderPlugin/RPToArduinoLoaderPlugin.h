#ifndef THE_BEST_PLUGIN_H
#define THE_BEST_PLUGIN_H

#include "../PluginAPI/PluginFunctions.h"

int loadOntoArduinoWithAvrdude(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath,
    const char* projectExecutableName);

#endif