#ifndef MAIN_FUNCTIONS_FILE_H
#define MAIN_FUNCTIONS_FILE_H

void setMainFunctionFile(const char* filepath);
const char* mainFunctionFilePath();

//The names include the prepending slash
const char* mainFunctionFileName();
const char* mainFunctionObjectFileName();

#endif