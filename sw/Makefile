EXE_NAME = GregBuild.exe
DIST_DIR = ../dist
USR_BIN_DIR = /usr/bin
DIST_EXE = $(DIST_DIR)/$(EXE_NAME)
USR_BIN_EXE = $(USR_BIN_DIR)/GB.exe
EXAMPLE_LIB = ../ExampleSrcRepo/lib
GREG_TEST_DLL = external/GregTest/dist/GregTest.dll

GregBuild: $(DIST_DIR) GregTestDll
	# Compile 
	gcc	$(wildcard core/application/*.c) \
		$(wildcard core/fileSystemRecursion/*.c) \
		$(wildcard core/main/*.c) \
		$(wildcard core/testMainWriting/*.c) \
		$(wildcard external/GregCToolkit/sw/CommandLineOptions/*.c) \
		$(wildcard external/GregCToolkit/sw/ExternalProgramExecution/*.c) \
		$(wildcard external/GregCToolkit/sw/FileSystem/*.c) \
		$(wildcard external/GregCToolkit/sw/String/*.c) \
		-o $(DIST_EXE)
	
	cp $(DIST_EXE) $(USR_BIN_EXE)

GregTestDll: $(EXAMPLE_LIB)
	cp $(GREG_TEST_DLL) $(EXAMPLE_LIB)/GregTest.dll

$(DIST_DIR):
	mkdir  -p $(DIST_DIR)

$(EXAMPLE_LIB):
	mkdir -p $(EXAMPLE_LIB)

clean:
	rm $(EXAMPLE_LIB) -r
	rm $(DIST_EXE)
	rm $(USR_BIN_EXE)