############################################################################
######						General Variables						  ######
############################################################################

BUILD=../build
CC=gcc
CFLAGS=-g
CORE=core
DIST=../dist
DIST_EXE=$(DIST)/$(EXE)
EXE=GB
LINKFLAGS=-ldl
TOOLKIT=GregCToolkit
TOOLKIT_SW = external/$(TOOLKIT)/sw
USR_BIN_EXE=/usr/bin/$(EXE)

#GregTest Variables
GREG_TEST_SO=GregTest.so
GREG_TEST_H=GregTest.h
GREG_TEST_USR_LIB_SO=/usr/lib/$(GREG_TEST_SO)
GREG_TEST_DIST_SO=external/GregTest/dist/$(GREG_TEST_SO)
GREG_TEST_USR_INCLUDE_H=/usr/include/$(GREG_TEST_H)
GREG_TEST_SW_H=external/GregTest/sw/$(GREG_TEST_H)
GREGTEST_MAKE_PATH=external/GregTest/sw

############################################################################
######						 GregBuild OBJS 						  ######
############################################################################

COMMON=common
COMMON_GLOBAL=$(COMMON)/global
COMPILER=compiler
FILE_SYSTEM_RECURSION=fileSystemRecursion
MAIN=main
PLUGIN_LOADER=pluginLoader
TEST_EXECUTION=testExecution

COMMON_OBJS = $(patsubst %.c,$(BUILD)/$(COMMON)/%.o,$(notdir $(shell find $(CORE)/$(COMMON) -maxdepth 1 -name "*.c")))
COMMON_GLOBAL_OBJS = $(patsubst %.c,$(BUILD)/$(COMMON_GLOBAL)/%.o,$(notdir $(shell find $(CORE)/$(COMMON_GLOBAL) -maxdepth 1 -name "*.c")))
COMPILER_OBJS = $(patsubst %.c,$(BUILD)/$(COMPILER)/%.o,$(notdir $(shell find $(CORE)/$(COMPILER) -maxdepth 1 -name "*.c")))
FILE_SYSTEM_RECURSION_OBJS = $(patsubst %.c,$(BUILD)/$(FILE_SYSTEM_RECURSION)/%.o,$(notdir $(shell find $(CORE)/$(FILE_SYSTEM_RECURSION) -maxdepth 1 -name "*.c")))
MAIN_OBJS = $(patsubst %.c,$(BUILD)/$(MAIN)/%.o,$(notdir $(shell find $(CORE)/$(MAIN) -maxdepth 1 -name "*.c")))
PLUGIN_LOADER_OBJS = $(patsubst %.c,$(BUILD)/$(PLUGIN_LOADER)/%.o,$(notdir $(shell find $(CORE)/$(PLUGIN_LOADER) -maxdepth 1 -name "*.c")))
TEST_EXECUTION_OBJS = $(patsubst %.c,$(BUILD)/$(TEST_EXECUTION)/%.o,$(notdir $(shell find $(CORE)/$(TEST_EXECUTION) -maxdepth 1 -name "*.c")))

GREG_BUILD_OBJS = $(COMMON_OBJS) \
				  $(COMMON_GLOBAL_OBJS) \
				  $(COMPILER_OBJS) \
				  $(FILE_SYSTEM_RECURSION_OBJS) \
				  $(MAIN_OBJS) \
                  $(PLUGIN_LOADER_OBJS) \
				  $(TEST_EXECUTION_OBJS)

############################################################################
######						  Toolkit OBJS 	    					  ######
############################################################################

ARG_LIST=ArgList
COLLECTIONS=Collections
COLLECTIONS_HASHTABLE=$(COLLECTIONS)/HashTable
COLLECTIONS_LINKEDLIST=$(COLLECTIONS)/LinkedList
COMMAND_LINE_OPTIONS=CommandLineOptions
EXTERNAL_PROGRAM_EXECUTION=ExternalProgramExecution
FAILURE_HANDLING=FailureHandling
FILE_SYSTEM=FileSystem
FILE_SYSTEM_FILE_IO=$(FILE_SYSTEM)/FileIO
STRING=String

ARG_LIST_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(ARG_LIST)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(ARG_LIST) -maxdepth 1 -name "*.c")))
COLLECTIONS_HASHTABLE_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(COLLECTIONS_HASHTABLE)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(COLLECTIONS_HASHTABLE) -maxdepth 1 -name "*.c")))
COLLECTIONS_LINKEDLIST_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(COLLECTIONS_LINKEDLIST)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(COLLECTIONS_LINKEDLIST) -maxdepth 1 -name "*.c")))
COMMAND_LINE_OPTIONS_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(COMMAND_LINE_OPTIONS)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(COMMAND_LINE_OPTIONS) -maxdepth 1 -name "*.c")))
EXTERNAL_PROGRAM_EXECUTION_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(EXTERNAL_PROGRAM_EXECUTION)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(EXTERNAL_PROGRAM_EXECUTION) -maxdepth 1 -name "*.c")))
FAILURE_HANDLING_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(FAILURE_HANDLING)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(FAILURE_HANDLING) -maxdepth 1 -name "*.c")))
FILE_SYSTEM_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(FILE_SYSTEM) -maxdepth 1 -name "*.c")))
FILE_SYSTEM_FILE_IO_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM_FILE_IO)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(FILE_SYSTEM_FILE_IO) -maxdepth 1 -name "*.c")))
STRING_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(STRING)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(STRING) -maxdepth 1 -name "*.c")))

TOOLKIT_OBJS =  $(ARG_LIST_OBJS) \
				$(COLLECTIONS_HASHTABLE_OBJS) \
				$(COLLECTIONS_LINKEDLIST_OBJS) \
				$(COMMAND_LINE_OPTIONS_OBJS) \
				$(EXTERNAL_PROGRAM_EXECUTION_OBJS) \
				$(FAILURE_HANDLING_OBJS) \
				$(FILE_SYSTEM_OBJS) \
				$(FILE_SYSTEM_FILE_IO_OBJS) \
				$(STRING_OBJS)

############################################################################
######						 Final Exe Rules						  ######
############################################################################

install: $(USR_BIN_EXE) $(GREG_TEST_USR_LIB_SO) $(GREG_TEST_USR_INCLUDE_H)

$(USR_BIN_EXE): $(DIST_EXE)
	sudo cp $< $@

$(DIST_EXE): $(TOOLKIT_OBJS) $(GREG_BUILD_OBJS)
	mkdir -p $(DIST)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKFLAGS)

$(GREG_TEST_USR_LIB_SO): $(GREG_TEST_DIST_SO)
	sudo cp $< $@

$(GREG_TEST_DIST_SO):
	cd $(GREGTEST_MAKE_PATH) && $(MAKE)

$(GREG_TEST_USR_INCLUDE_H): $(GREG_TEST_SW_H)
	sudo cp $< $@

############################################################################
######						GregBuild OBJ Rules						  ######
############################################################################

$(BUILD)/$(COMMON)/%.o: $(CORE)/$(COMMON)/%.c $(CORE)/$(COMMON)/%.h
	mkdir -p $(BUILD)/$(COMMON)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(COMMON_GLOBAL)/%.o: $(CORE)/$(COMMON_GLOBAL)/%.c $(CORE)/$(COMMON_GLOBAL)/%.h
	mkdir -p $(BUILD)/$(COMMON_GLOBAL)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(COMPILER)/%.o: $(CORE)/$(COMPILER)/%.c $(CORE)/$(COMPILER)/%.h
	mkdir -p $(BUILD)/$(COMPILER)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(FILE_SYSTEM_RECURSION)/%.o: $(CORE)/$(FILE_SYSTEM_RECURSION)/%.c $(CORE)/$(FILE_SYSTEM_RECURSION)/%.h
	mkdir -p $(BUILD)/$(FILE_SYSTEM_RECURSION)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(MAIN)/%.o: $(CORE)/$(MAIN)/%.c
	mkdir -p $(BUILD)/$(MAIN)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(PLUGIN_LOADER)/%.o: $(CORE)/$(PLUGIN_LOADER)/%.c $(CORE)/$(PLUGIN_LOADER)/%.h
	mkdir -p $(BUILD)/$(PLUGIN_LOADER)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TEST_EXECUTION)/%.o: $(CORE)/$(TEST_EXECUTION)/%.c $(CORE)/$(TEST_EXECUTION)/%.h
	mkdir -p $(BUILD)/$(TEST_EXECUTION)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

############################################################################
######						Toolkit OBJ Rules						  ######
############################################################################

$(BUILD)/$(TOOLKIT)/$(ARG_LIST)/%.o: $(TOOLKIT_SW)/$(ARG_LIST)/%.c $(TOOLKIT_SW)/$(ARG_LIST)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(ARG_LIST)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(COLLECTIONS_HASHTABLE)/%.o: $(TOOLKIT_SW)/$(COLLECTIONS_HASHTABLE)/%.c $(TOOLKIT_SW)/$(COLLECTIONS_HASHTABLE)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(COLLECTIONS_HASHTABLE)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(COLLECTIONS_LINKEDLIST)/%.o: $(TOOLKIT_SW)/$(COLLECTIONS_LINKEDLIST)/%.c $(TOOLKIT_SW)/$(COLLECTIONS_LINKEDLIST)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(COLLECTIONS_LINKEDLIST)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(COMMAND_LINE_OPTIONS)/%.o: $(TOOLKIT_SW)/$(COMMAND_LINE_OPTIONS)/%.c $(TOOLKIT_SW)/$(COMMAND_LINE_OPTIONS)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(COMMAND_LINE_OPTIONS)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(EXTERNAL_PROGRAM_EXECUTION)/%.o: $(TOOLKIT_SW)/$(EXTERNAL_PROGRAM_EXECUTION)/%.c $(TOOLKIT_SW)/$(EXTERNAL_PROGRAM_EXECUTION)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(EXTERNAL_PROGRAM_EXECUTION)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(FAILURE_HANDLING)/%.o: $(TOOLKIT_SW)/$(FAILURE_HANDLING)/%.c $(TOOLKIT_SW)/$(FAILURE_HANDLING)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(FAILURE_HANDLING)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM)/%.o: $(TOOLKIT_SW)/$(FILE_SYSTEM)/%.c $(TOOLKIT_SW)/$(FILE_SYSTEM)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM_FILE_IO)/%.o: $(TOOLKIT_SW)/$(FILE_SYSTEM_FILE_IO)/%.c $(TOOLKIT_SW)/$(FILE_SYSTEM_FILE_IO)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM_FILE_IO)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@	

$(BUILD)/$(TOOLKIT)/$(STRING)/%.o: $(TOOLKIT_SW)/$(STRING)/%.c $(TOOLKIT_SW)/$(STRING)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(STRING)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

############################################################################
######						Plugin Rules	     					  ######
############################################################################

Plugins: ../lib/plugins/TimeTrackingPlugin.so ../lib/plugins/RPToArduinoLoaderPlugin.so

../lib/plugins/TimeTrackingPlugin.so: plugins/TimeTrackingPlugin/TimeTrackingPlugin.c plugins/TimeTrackingPlugin/TimeTrackingPlugin.h
	mkdir -p ../lib/plugins
	mkdir -p ../ExampleSrcRepo/lib/plugins
	mkdir -p ../Blink/lib/plugins
	mkdir -p ~/ExampleSrcRepo/lib/plugins
	mkdir -p ~/Blink/lib/plugins
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^
	cp $@ ../ExampleSrcRepo/lib/plugins
	cp $@ ~/ExampleSrcRepo/lib/plugins
	cp $@ ~/Blink/lib/plugins

../lib/plugins/RPToArduinoLoaderPlugin.so: plugins/RPToArduinoLoaderPlugin/RPToArduinoLoaderPlugin.c plugins/RPToArduinoLoaderPlugin/RPToArduinoLoaderPlugin.h
	mkdir -p ../lib/plugins
	mkdir -p ../ExampleSrcRepo/lib/plugins
	mkdir -p ../Blink/lib/plugins
	mkdir -p ~/ExampleSrcRepo/lib/plugins
	mkdir -p ~/Blink/lib/plugins
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^
	cp $@ ../ExampleSrcRepo/lib/plugins
	cp $@ ../Blink/lib/plugins
	cp $@ ~/ExampleSrcRepo/lib/plugins
	cp $@ ~/Blink/lib/plugins

############################################################################
######						Clean Rules								  ######
############################################################################

clean:
	rm $(BUILD) -r
	rm $(DIST) -r
	sudo rm $(USR_BIN_EXE)

cleanPlugins:
	rm ../lib/plugins/TimeTrackingPlugin.so
	rm ../lib/plugins/TheBestPlugin.so
	rm ~/ExampleSrcRepo/lib/plugins -r
	rm ~/Blink/lib/plugins -r