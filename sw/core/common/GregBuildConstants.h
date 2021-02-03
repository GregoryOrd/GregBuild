#ifndef GREG_TEST_CONSTANTS_H
#define GREG_TEST_CONSTANTS_H

#define TEST_PROJECT_DLL "TestProject.dll"
#define GREG_TEST_DLL "GregTest.dll"
#define LIB_GREG_TEST_DLL "lib/GregTest.dll"

#define CURRENT_DIR "."

#define TEMP_DIR "temp"
#define TEMP_TEST_PROJECT_DLL "temp/TestProject.dll"
#define TEMP_TEST_MAIN "temp/TestMain"
#define TEMP_TEST_MAIN_C "temp/TestMain.c"
#define TEMP_TEST_MAIN_H "temp/TestMain.h"
#define TEMP_TEST_MAIN_EXE "temp/TestMain.exe"

#define DIST "dist"
#define PROJECT_EXE "dist/HelloWorld.exe"

#define SRC_DIR "src"

#define NUM_SUPPORTED_COMMAND_LINE_OPTIONS 2

#define NO_TEST_OPTION_TEXT "--no-test-build"
#define NO_TEST_DESCRIPTION "Don't Run Any Tests"
#define NO_TEST_FLAG_VALUE 1 // The value 1 here means run the tests

#define DELETE_TEMP_DIR_OPTION_TEXT "--leave-temp"
#define DELETE_TEMP_DIR_DESCRIPTION "Don't Delete the temp Directory"
#define DELETE_TEMP_DIR_FLAG_VALUE                                             \
  1 // The value 1 here means delete the temp directory

#define BUILD_SEQUENCE_STEP_TYPE 1
#define COMMAND_LINE_OPTION_TYPE 2
#define HMODULE_LL_TYPE 3

#define PLUGINS_LIB_DIRECTORY "./lib/plugins"
#define PLUGINS_LOAD_ORDER_CONFIG_FILE PLUGINS_LIB_DIRECTORY"/order"

#endif