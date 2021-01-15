EXE_NAME = GregBuild.exe
DIST_DIR = ../dist
USR_BIN_DIR = /usr/bin
TOOLKIT_LIB = external/GregCToolkit/lib
DIST_EXE = $(DIST_DIR)/$(EXE_NAME)
USR_BIN_EXE = $(USR_BIN_DIR)/GB.exe
EXAMPLE_LIB = ../ExampleSrcRepo/lib
GREG_TEST_DLL = external/GregTest/dist/GregTest.dll
SOURCES := $(shell find . -name "*.c" ! -path */external/*)
TOOLKIT_SOURCES := $(shell find external/GregCToolkit -name "*.c")

GregBuild: $(DIST_DIR) GregTestDll GregCToolkit
	# Compile GregBuild 
	gcc	$(SOURCES) $(wildcard $(TOOLKIT_LIB)/*.o) -o $(DIST_EXE)
	cp $(DIST_EXE) $(USR_BIN_EXE)

GregCToolkit: $(TOOLKIT_LIB)
	# Compile GregCToolkit
	gcc -c $(TOOLKIT_SOURCES)
	mv *.o $(TOOLKIT_LIB)

GregTestDll: $(EXAMPLE_LIB)
	cp $(GREG_TEST_DLL) $(EXAMPLE_LIB)/GregTest.dll

$(DIST_DIR):
	mkdir  -p $(DIST_DIR)

$(EXAMPLE_LIB):
	mkdir -p $(EXAMPLE_LIB)

$(TOOLKIT_LIB):
	mkdir -p $(TOOLKIT_LIB)

clean:
	rm $(TOOLKIT_LIB) -r
	rm $(EXAMPLE_LIB) -r
	rm $(USR_BIN_EXE)
	rm $(DIST_EXE)
