############################################################################
######						General Variables						  ######
############################################################################

CORE=core
BUILD=../build
EXE=GB.exe
DIST=../dist
DIST_EXE=$(DIST)/$(EXE)
USR_BIN_EXE=/usr/bin/$(EXE)
TOOLKIT=GregCToolkit
TOOLKIT_SW = external/$(TOOLKIT)/sw
CC=gcc
CFLAGS=-g

############################################################################
######						 GregBuild OBJS 						  ######
############################################################################

APPLICATION=application
FILE_SYSTEM_RECURSION=fileSystemRecursion
MAIN=main
TEST_MAIN_WRITING=testMainWriting
PLUGIN_LOADER=pluginLoader
COMMON=common

APPLICATION_OBJS = $(patsubst %.c,$(BUILD)/$(APPLICATION)/%.o,$(notdir $(shell find $(CORE)/$(APPLICATION) -name "*.c")))
COMMON_OBJS = $(patsubst %.c,$(BUILD)/$(COMMON)/%.o,$(notdir $(shell find $(CORE)/$(COMMON) -name "*.c")))
FILE_SYSTEM_RECURSION_OBJS = $(patsubst %.c,$(BUILD)/$(FILE_SYSTEM_RECURSION)/%.o,$(notdir $(shell find $(CORE)/$(FILE_SYSTEM_RECURSION) -name "*.c")))
MAIN_OBJS = $(patsubst %.c,$(BUILD)/$(MAIN)/%.o,$(notdir $(shell find $(CORE)/$(MAIN) -name "*.c")))
TEST_MAIN_WRITING_OBJS = $(patsubst %.c,$(BUILD)/$(TEST_MAIN_WRITING)/%.o,$(notdir $(shell find $(CORE)/$(TEST_MAIN_WRITING) -name "*.c")))
PLUGIN_LOADER_OBJS = $(patsubst %.c,$(BUILD)/$(PLUGIN_LOADER)/%.o,$(notdir $(shell find $(CORE)/$(PLUGIN_LOADER) -name "*.c")))

GREG_BUILD_OBJS = $(APPLICATION_OBJS) \
				  $(COMMON_OBJS) \
				  $(FILE_SYSTEM_RECURSION_OBJS) \
				  $(MAIN_OBJS) \
				  $(TEST_MAIN_WRITING_OBJS) \
                  $(PLUGIN_LOADER_OBJS)

############################################################################
######						  Toolkit OBJS 	    					  ######
############################################################################

COLLECTIONS=Collections
COMMAND_LINE_OPTIONS=CommandLineOptions
EXTERNAL_PROGRAM_EXECUTION=ExternalProgramExecution
FAILURE_HANDLING=FailureHandling
FILE_IO=FileIO
FILE_SYSTEM=FileSystem
STRING=String

COLLECTIONS_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(COLLECTIONS)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(COLLECTIONS) -name "*.c")))
COMMAND_LINE_OPTIONS_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(COMMAND_LINE_OPTIONS)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(COMMAND_LINE_OPTIONS) -name "*.c")))
EXTERNAL_PROGRAM_EXECUTION_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(EXTERNAL_PROGRAM_EXECUTION)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(EXTERNAL_PROGRAM_EXECUTION) -name "*.c")))
FAILURE_HANDLING_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(FAILURE_HANDLING)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(FAILURE_HANDLING) -name "*.c")))
FILE_IO_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(FILE_IO)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(FILE_IO) -name "*.c")))
FILE_SYSTEM_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(FILE_SYSTEM) -name "*.c")))
STRING_OBJS = $(patsubst %.c,$(BUILD)/$(TOOLKIT)/$(STRING)/%.o,$(notdir $(shell find $(TOOLKIT_SW)/$(STRING) -name "*.c")))

TOOLKIT_OBJS =  $(COLLECTIONS_OBJS) \
				$(COMMAND_LINE_OPTIONS_OBJS) \
				$(EXTERNAL_PROGRAM_EXECUTION_OBJS) \
				$(FAILURE_HANDLING_OBJS) \
				$(FILE_IO_OBJS) \
				$(FILE_SYSTEM_OBJS) \
				$(STRING_OBJS)

############################################################################
######						 Final Exe Rules						  ######
############################################################################

$(USR_BIN_EXE): $(DIST_EXE)
	cp $(DIST_EXE) $(USR_BIN_EXE)

$(DIST_EXE): $(GREG_BUILD_OBJS) $(TOOLKIT_OBJS)
	mkdir -p $(DIST)
	$(CC) $(CFLAGS) -o $@ $^


############################################################################
######						GregBuild OBJ Rules						  ######
############################################################################

$(BUILD)/$(APPLICATION)/%.o: $(CORE)/$(APPLICATION)/%.c $(CORE)/$(APPLICATION)/%.h
	mkdir -p $(BUILD)/$(APPLICATION)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(COMMON)/%.o: $(CORE)/$(COMMON)/%.c $(CORE)/$(COMMON)/%.h
	mkdir -p $(BUILD)/$(COMMON)
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

$(BUILD)/$(TEST_MAIN_WRITING)/%.o: $(CORE)/$(TEST_MAIN_WRITING)/%.c $(CORE)/$(TEST_MAIN_WRITING)/%.h
	mkdir -p $(BUILD)/$(TEST_MAIN_WRITING)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(PLUGIN_LOADER)/%.o: $(CORE)/$(PLUGIN_LOADER)/%.c $(CORE)/$(PLUGIN_LOADER)/%.h
	mkdir -p $(BUILD)/$(PLUGIN_LOADER)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

############################################################################
######						Toolkit OBJ Rules						  ######
############################################################################

$(BUILD)/$(TOOLKIT)/$(COLLECTIONS)/%.o: $(TOOLKIT_SW)/$(COLLECTIONS)/%.c $(TOOLKIT_SW)/$(COLLECTIONS)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(COLLECTIONS)
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

$(BUILD)/$(TOOLKIT)/$(FILE_IO)/%.o: $(TOOLKIT_SW)/$(FILE_IO)/%.c $(TOOLKIT_SW)/$(FILE_IO)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(FILE_IO)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@	

$(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM)/%.o: $(TOOLKIT_SW)/$(FILE_SYSTEM)/%.c $(TOOLKIT_SW)/$(FILE_SYSTEM)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(FILE_SYSTEM)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

$(BUILD)/$(TOOLKIT)/$(STRING)/%.o: $(TOOLKIT_SW)/$(STRING)/%.c $(TOOLKIT_SW)/$(STRING)/%.h
	mkdir -p $(BUILD)/$(TOOLKIT)/$(STRING)
	$(CC) $(CFLAGS) -c $<
	mv $(notdir $@) $@

############################################################################
######						Plugin Rules	     					  ######
############################################################################

Plugins: ../lib/plugins/TimeTrackingPlugin.dll ../lib/plugins/TheBestPlugin.dll

../lib/plugins/TimeTrackingPlugin.dll: plugins/TimeTrackingPlugin/TimeTrackingPlugin.c plugins/TimeTrackingPlugin/TimeTrackingPlugin.h
	mkdir -p ../lib/plugins
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^
	cp $@ ../ExampleSrcRepo/lib/plugins

../lib/plugins/TheBestPlugin.dll: plugins/TheBestPlugin/TheBestPlugin.c plugins/TheBestPlugin/TheBestPlugin.h
	mkdir -p ../lib/plugins
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^
	cp $@ ../ExampleSrcRepo/lib/plugins

############################################################################
######						Clean Rules								  ######
############################################################################

clean:
	rm $(BUILD) -r
	rm $(DIST) -r
	rm $(USR_BIN_EXE)

cleanPlugins:
	rm ../lib/plugins/TimeTrackingPlugin.dll
	rm ../lib/plugins/TheBestPlugin.dll
