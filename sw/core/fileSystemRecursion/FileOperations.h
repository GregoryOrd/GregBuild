#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "../common/FileStructureDefs.h"

void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void initTestFiles(TestFileList* testFiles);
void initSourceFiles(SourceFileList* sourceFiles);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void freeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeTestFileList(TestFileList* list);
void freeSourceFileList(SourceFileList* list);
int removeTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath);

#endif