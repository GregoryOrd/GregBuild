EXE_NAME = GregBuild.exe
DIST_DIR = ../dist
USR_BIN_DIR = /usr/bin
TOOLKIT_LIB = external/GregCToolkit/lib
DIST_EXE = $(DIST_DIR)/$(EXE_NAME)
USR_BIN_EXE = $(USR_BIN_DIR)/GB.exe
EXAMPLE_LIB = ../ExampleSrcRepo/lib
GREG_TEST_DLL = GregTest.dll
EXTERNAL_GREG_TEST_DLL = external/GregTest/dist/$(GREG_TEST_DLL)
EXAMPLE_LIB_GREG_TEST_DLL = $(EXAMPLE_LIB)/$(GREG_TEST_DLL)
SOURCES := $(shell find . -name "*.c" ! -path */external/*)
TOOLKIT_SOURCES := $(shell find external/GregCToolkit -name "*.c")
PLUGINS_DIR = $(DIST_DIR)/Plugins
TIME_TRACKING_PLUGIN_SOURCES := $(shell find plugins/TimeTrackingPlugin -name "*.c")

GregBuild: $(DIST_DIR) $(EXAMPLE_LIB_GREG_TEST_DLL) GregCToolkit
	# Compile GregBuild 
	gcc	$(SOURCES) $(wildcard $(TOOLKIT_LIB)/*.o) -o $(DIST_EXE)
	cp $(DIST_EXE) $(USR_BIN_EXE)

GregCToolkit: $(TOOLKIT_LIB)
	# Compile GregCToolkit
	gcc -c $(TOOLKIT_SOURCES)
	mv *.o $(TOOLKIT_LIB)

$(EXAMPLE_LIB_GREG_TEST_DLL): $(EXAMPLE_LIB)
	#Copying GregTest.dll to ../ExampleSrcRepo/lib
	cp $(EXTERNAL_GREG_TEST_DLL) $(EXAMPLE_LIB_GREG_TEST_DLL)

Plugins: $(PLUGINS_DIR)
	gcc -shared $(TIME_TRACKING_PLUGIN_SOURCES) -o $(PLUGINS_DIR)/TimeTrackingPlugin.dll

$(DIST_DIR):
	mkdir  -p $(DIST_DIR)

$(EXAMPLE_LIB):
	mkdir -p $(EXAMPLE_LIB)

$(TOOLKIT_LIB):
	mkdir -p $(TOOLKIT_LIB)

$(PLUGINS_DIR):
	mkdir -p $(PLUGINS_DIR)

clean:
	rm $(TOOLKIT_LIB) -r
	rm $(EXAMPLE_LIB) -r
	rm $(USR_BIN_EXE)
	rm $(DIST_EXE)
