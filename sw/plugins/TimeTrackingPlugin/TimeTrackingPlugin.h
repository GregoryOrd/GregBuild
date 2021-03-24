#ifndef TIME_TRACKING_PLUGIN_H
#define TIME_TRACKING_PLUGIN_H

#include "../PluginAPI/PluginFunctions.h"

int printHelloWorld(const TestFileList* testFiles, const SourceFileList* sourceFiles,
                    const ObjectFileList* tempObjectFiles, int errorOnPreviousStep,
                    const char* basePath, const char* projectExecutableName);

#endif