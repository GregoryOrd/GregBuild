#ifndef TIME_TRACKING_PLUGIN_H
#define TIME_TRACKING_PLUGIN_H

#include "../PluginAPI/PluginFunctions.h"

int printHelloWorld(TestFileList *testFiles, SourceFileList *sourceFiles,
                    ObjectFileList *tempObjectFiles, int errorOnPreviousStep,
                    char *basePath);

#endif