EXE_NAME = GregBuild.exe
DIST_DIR = ../dist
USR_BIN_DIR = /usr/bin
DIST_EXE = $(DIST_DIR)/$(EXE_NAME)
USR_BIN_EXE = $(USR_BIN_DIR)/$(EXE_NAME)
EXAMPLE_LIB = ../ExampleSrcRepo/lib
GREG_TEST_DLL = external/GregTest/dist/GregTest.dll

GregBuild: $(DIST_DIR) GregTestDll
	# Compile 
	gcc core/GregBuildMain.c \
		core/StringUtils.c \
		core/FileSystemOperations.c \
		core/CompileAndLinkCommands.c \
		core/RunTests.c \
		fileSystemRecursion/FileAndTestCaseGatherer.c \
		fileSystemRecursion/TestAndSrcDefinitions.c \
		testMainWriting/TestMainWriter.c \
		commandLineCalls/ExternalProgramExecution.c \
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