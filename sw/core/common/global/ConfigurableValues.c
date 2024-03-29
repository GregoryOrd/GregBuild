#include "ConfigurableValues.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../../../external/GregCToolkit/sw/Collections/HashTable/HashTable.h"
#include "../../../external/GregCToolkit/sw/FileSystem/FileIO/FileReader.h"
#include "../../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../../common/FileStructureDefs.h"
#include "../../common/GregBuildConstants.h"

//////////////////////////////////////////////////////////////////////
//              Private Data and Function Prototypes                //
//////////////////////////////////////////////////////////////////////

#define NUM_COMPILER_CONFIG_PARAMS    14
#define SET_CONFIGURATION_TYPE        1
#define actionPerConfigurationSetting 2
#define dataMembersPerAction          1

typedef int (*SET_CONFIGURATION_ACTION)(void**);
typedef struct SetConfiguration
{
   SET_CONFIGURATION_ACTION actions[actionPerConfigurationSetting];
   void* dataToActOn[dataMembersPerAction * actionPerConfigurationSetting];

} SetConfiguration;

static void setDataToActOn(int i);
static HashTable* setupConfigurationsHashTable();
static int string_copy(void** args);
static void freeSetupConfigurations();
static int parseConfigurationFileLine(ArgList* argList);
static void parseParamAndValueFromBuffer(char* param, char* value, const char* buffer);
static void addCharToParamIfBeforeDelimiter(int i, char* param, const char* buffer, bool* delimiterReached, int* indexOfDelimiter);
static void addCharToValueIfAfterDelimiter(int i, char* value, const char* buffer, bool delimiterReached, int indexOfDelimiter);
static void setConfigurations(const char* param, const char* value);
static void setConfigurationForSingleParameter(HashTable* table, const char* param, const char* value);
static void executeActionOnDataWithValue(SET_CONFIGURATION_ACTION action, void* data, const char* value);
static void initOptionLists();
static void initHostCompilerOptionsList();
static void initTargetCompilerOptionsList();
static void initHostLinkerOptionsList();
static void initTargetLinkerOptionsList();
static void initHostExcludedFilesList();
static void initTargetExcludedFilesList();
static void initHardwareSimulationLibrariesList();
static void freeStringData(void* data);
static int listTypeFromData(void* data);

static const char* compilerConfigParams[NUM_COMPILER_CONFIG_PARAMS] = {
    "host",
    "target",
    "executableName",
    "compilerOption",
    "hostCompilerOption",
    "targetCompilerOption",
    "linkerOption",
    "hostLinkerOption",
    "targetLinkerOption",
    "hostExcludedFile",
    "targetExcludedFile",
    "buildStartingDirectory",
    "testFrameworkLibrary",
    "hardwareSimulationLibrary"};

static SetConfiguration configurations[NUM_COMPILER_CONFIG_PARAMS] = {
    {.actions = {string_copy, NULL}, .dataToActOn = {hostCompiler_, NULL}},                            // (0) host
    {.actions = {string_copy, NULL}, .dataToActOn = {targetCompiler_, NULL}},                          // (1) target
    {.actions = {string_copy, NULL}, .dataToActOn = {projectExecutableName_, NULL}},                   // (2) executableName
    {.actions = {append_string_voidArgs_ll, append_string_voidArgs_ll}, .dataToActOn = {NULL, NULL}},  // (3) compilerOption
    {.actions = {append_string_voidArgs_ll, NULL}, .dataToActOn = {NULL}},                             // (4) hostCompilerOption
    {.actions = {append_string_voidArgs_ll, NULL}, .dataToActOn = {NULL}},                             // (5) targetCompilerOption
    {.actions = {append_string_voidArgs_ll, append_string_voidArgs_ll}, .dataToActOn = {NULL, NULL}},  // (6) compilerOption
    {.actions = {append_string_voidArgs_ll, NULL}, .dataToActOn = {NULL}},                             // (7) hostLinkerOption
    {.actions = {append_string_voidArgs_ll, NULL}, .dataToActOn = {NULL}},                             // (8) targetLinkerOption
    {.actions = {append_string_voidArgs_ll, NULL}, .dataToActOn = {NULL}},                             // (9) hostExcludedFiles
    {.actions = {append_string_voidArgs_ll, NULL}, .dataToActOn = {NULL}},                             // (10) targetExcludedFiles
    {.actions = {string_copy, NULL}, .dataToActOn = {buildStartingDirectory_, NULL}},                  // (11) buildStartingDirectory
    {.actions = {string_copy, NULL}, .dataToActOn = {testFrameworkLibrary_, NULL}},                    // (12) testFrameworkLibrary
    {.actions = {append_string_voidArgs_ll, NULL}, .dataToActOn = {NULL}},                             // (13) hardwareSimulationLibrary
};

//////////////////////////////////////////////////////////////////////
//              Function Implementation Section                     //
//////////////////////////////////////////////////////////////////////

int readConfigurationsFromFile()
{
   initOptionLists();
   initHostExcludedFilesList();
   initTargetExcludedFilesList();
   initHardwareSimulationLibrariesList();

   HashTable* table = setupConfigurationsHashTable();
   ArgList* argList = malloc(sizeof(ArgList));
   argList->size = 1;
   argList->args = malloc(sizeof(void*));
   argList->args[0] = table;
   readFileWithActionAfterEachLine(CONFIG_FILE, argList, parseConfigurationFileLine);

   freeHashTable(table, freeSetupConfigurations, false, false);
   freeArgList(argList, false);
   return 0;
}

int parseConfigurationFileLine(ArgList* argList)
{
   char buffer[WINDOWS_MAX_PATH_LENGTH] = "";
   char param[WINDOWS_MAX_PATH_LENGTH] = "";
   char value[WINDOWS_MAX_PATH_LENGTH] = "";

   HashTable* table = argList->args[0];
   strcpy(buffer, (char*)argList->args[argList->size - 1]);
   parseParamAndValueFromBuffer(param, value, buffer);
   setConfigurationForSingleParameter(table, param, value);

   return 0;
}

void parseParamAndValueFromBuffer(char* param, char* value, const char* buffer)
{
   bool delimiterReached = false;
   bool newLineReached = false;
   int indexOfDelimiter = 0;
   for (int i = 0; i < strlen(buffer); i++)
   {
      addCharToParamIfBeforeDelimiter(i, param, buffer, &delimiterReached, &indexOfDelimiter);
      addCharToValueIfAfterDelimiter(i, value, buffer, delimiterReached, indexOfDelimiter);
   }
}

void addCharToParamIfBeforeDelimiter(int i, char* param, const char* buffer, bool* delimiterReached, int* indexOfDelimiter)
{
   if (buffer[i] != COMPILER_CONFIG_DELIMITER && !(*delimiterReached))
   {
      param[i] = buffer[i];
   }
   else if (buffer[i] == COMPILER_CONFIG_DELIMITER)
   {
      *delimiterReached = true;
      *indexOfDelimiter = i;
      i++;
   }
}

void addCharToValueIfAfterDelimiter(int i, char* value, const char* buffer, bool delimiterReached, int indexOfDelimiter)
{
   if (delimiterReached)
   {
      value[i - indexOfDelimiter - 1] = buffer[i];
   }
}

void setConfigurationForSingleParameter(HashTable* table, const char* param, const char* value)
{
   SetConfiguration* setConfiguration = (SetConfiguration*)hash_lookup(table, param, SET_CONFIGURATION_TYPE);
   if (setConfiguration == NULL)
   {
      return;
   }

   for (int actionNum = 0; actionNum < actionPerConfigurationSetting; actionNum++)
   {
      SET_CONFIGURATION_ACTION action = setConfiguration->actions[actionNum];
      void* data = setConfiguration->dataToActOn[actionNum];  // Action #1 uses Data #1, Action #2 uses Data #2, etc.
      executeActionOnDataWithValue(action, data, value);
   }
}

void executeActionOnDataWithValue(SET_CONFIGURATION_ACTION action, void* data, const char* value)
{
   if (action == NULL || data == NULL)
   {
      return;
   }

   void* args[3];
   args[0] = data;
   args[1] = (void*)value;

   // If the data is not a list, the action will not use the listType argument
   int listType = listTypeFromData(data);
   args[2] = &listType;

   action(args);
}

int listTypeFromData(void* data)
{
   if (data == hostCompilerOptions_ || data == targetCompilerOptions_)
   {
      return COMPILER_OPTION_TYPE;
   }
   if (data == hostLinkerOptions_ || data == targetLinkerOptions_)
   {
      return LINKER_OPTION_TYPE;
   }
   else if (data == hostExcludedFiles_)
   {
      return HOST_EXCLUDED_FILE_TYPE;
   }
   else if (data == targetExcludedFiles_)
   {
      return TARGET_EXCLUDED_FILE_TYPE;
   }
   else if (data == hardwareSimulationLibraries_)
   {
      return HARDWARE_SIMULATION_LIBRARY;
   }
}

void freeSetupConfigurations() {}

HashTable* setupConfigurationsHashTable()
{
   HashTable* table = malloc(sizeof(HashTable));
   initHashTable(table, SET_CONFIGURATION_TYPE, NUM_COMPILER_CONFIG_PARAMS);
   for (int i = 0; i < NUM_COMPILER_CONFIG_PARAMS; i++)
   {
      setDataToActOn(i);
      HashTableItem* item = malloc(sizeof(HashTableItem));
      item->key = compilerConfigParams[i];
      item->data = &configurations[i];
      hash_insert(table, item, SET_CONFIGURATION_TYPE);
   }
   return table;
}

void setDataToActOn(int i)
{
   switch (i)
   {
      case 3:  // compilerOption
         configurations[i].dataToActOn[0] = (void*)hostCompilerOptions_;
         configurations[i].dataToActOn[1] = (void*)targetCompilerOptions_;
         break;
      case 4:  // hostCompilerOption
         configurations[i].dataToActOn[0] = (void*)hostCompilerOptions_;
         break;
      case 5:  // targetCompilerOption
         configurations[i].dataToActOn[0] = (void*)targetCompilerOptions_;
         break;
      case 6:  // linkerOption
         configurations[i].dataToActOn[0] = (void*)hostLinkerOptions_;
         configurations[i].dataToActOn[1] = (void*)targetLinkerOptions_;
         break;
      case 7:  // hostLinkerOption
         configurations[i].dataToActOn[0] = (void*)hostLinkerOptions_;
         break;
      case 8:  // targetLinkerOption
         configurations[i].dataToActOn[0] = (void*)targetLinkerOptions_;
         break;
      case 9:  // hostExcludedFiles
         configurations[i].dataToActOn[0] = (void*)hostExcludedFiles_;
         break;
      case 10:  // targetExcludedFiles
         configurations[i].dataToActOn[0] = (void*)targetExcludedFiles_;
         break;
      case 13:  // hardwareSimulationLibraries
         configurations[i].dataToActOn[0] = (void*)hardwareSimulationLibraries_;
         break;
   }
}

int string_copy(void** args)
{
   char* configurationValue = (char*)args[0];
   const char* value = (const char*)args[1];
   strcpy(configurationValue, value);
   return 0;
}

void initOptionLists()
{
   initHostCompilerOptionsList();
   initTargetCompilerOptionsList();
   initHostLinkerOptionsList();
   initTargetLinkerOptionsList();
}

void initHostCompilerOptionsList()
{
   hostCompilerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(hostCompilerOptions_, COMPILER_OPTION_TYPE);
}

void initTargetCompilerOptionsList()
{
   targetCompilerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(targetCompilerOptions_, COMPILER_OPTION_TYPE);
}

void initHostLinkerOptionsList()
{
   hostLinkerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(hostLinkerOptions_, LINKER_OPTION_TYPE);
}

void initTargetLinkerOptionsList()
{
   targetLinkerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(targetLinkerOptions_, LINKER_OPTION_TYPE);
}

void initHostExcludedFilesList()
{
   hostExcludedFiles_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(hostExcludedFiles_, HOST_EXCLUDED_FILE_TYPE);
}

void initTargetExcludedFilesList()
{
   targetExcludedFiles_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(targetExcludedFiles_, TARGET_EXCLUDED_FILE_TYPE);
}

void initHardwareSimulationLibrariesList()
{
   hardwareSimulationLibraries_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(hardwareSimulationLibraries_, HARDWARE_SIMULATION_LIBRARY);
}

void freeGlobalOptionsLists()
{
   freeLinkedList(hostCompilerOptions_, freeStringData);
   freeLinkedList(targetCompilerOptions_, freeStringData);
   freeLinkedList(hostLinkerOptions_, freeStringData);
   freeLinkedList(targetLinkerOptions_, freeStringData);
}

void freeHostExcludedFilesList() { freeLinkedList(hostExcludedFiles_, freeStringData); }

void freeTargetExcludedFilesList() { freeLinkedList(targetExcludedFiles_, freeStringData); }

void freeHardwareSimulationLibrariesList() { freeLinkedList(hardwareSimulationLibraries_, freeStringData); }

void freeStringData(void* data) { free(data); }

const char* hostCompiler() { return hostCompiler_; }

const char* targetCompiler() { return targetCompiler_; }

const char* projectExecutableName() { return projectExecutableName_; }

const char* buildStartingDirectory() { return buildStartingDirectory_; }

const char* testFrameworkLibrary() { return testFrameworkLibrary_; }

LinkedList* hostCompilerOptions()
{
   if (hostCompilerOptions_ == NULL)
   {
      initHostCompilerOptionsList();
   }
   return hostCompilerOptions_;
}

LinkedList* targetCompilerOptions()
{
   if (targetCompilerOptions_ == NULL)
   {
      initTargetCompilerOptionsList();
   }
   return targetCompilerOptions_;
}

LinkedList* hostLinkerOptions()
{
   if (hostLinkerOptions_ == NULL)
   {
      initHostLinkerOptionsList();
   }
   return hostLinkerOptions_;
}

LinkedList* targetLinkerOptions()
{
   if (targetLinkerOptions_ == NULL)
   {
      initTargetLinkerOptionsList();
   }
   return targetLinkerOptions_;
}

LinkedList* hostExcludedFiles()
{
   if (hostExcludedFiles_ == NULL)
   {
      initHostExcludedFilesList();
   }
   return hostExcludedFiles_;
}

LinkedList* targetExcludedFiles()
{
   if (targetExcludedFiles_ == NULL)
   {
      initTargetExcludedFilesList();
   }
   return targetExcludedFiles_;
}

LinkedList* hardwareSimulationLibraries()
{
   if (hardwareSimulationLibraries_ == NULL)
   {
      initHardwareSimulationLibrariesList();
   }
   return hardwareSimulationLibraries_;
}