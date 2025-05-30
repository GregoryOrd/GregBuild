#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "../common/FileStructureDefs.h"

#ifdef __cplusplus
extern "C" {
#endif  

void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void initTestFiles(TestFileList* testFiles);
void initSourceFiles(SourceFileList* sourceFiles);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void resetObjectFileList(ObjectFileList* list);
void freeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeTestFileList(TestFileList* list);
void freeSourceFileList(SourceFileList* list);
int cleanTempDir(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
int makeTempDir();

#ifdef __cplusplus
}
#endif  

#endif