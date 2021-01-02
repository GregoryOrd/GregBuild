distDirectory = ../dist
libDirectory = "../ExampleSrcRepo/lib"

GregBuild: $(distDirectory) GregTestDll
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
		-o ../dist/GregBuild.exe
	
	cp ../dist/GregBuild.exe ../ExampleSrcRepo

GregTestDll: $(libDirectory)
	cp external/GregTest/dist/GregTest.dll ../ExampleSrcRepo/lib/GregTest.dll

$(distDirectory):
	mkdir ../dist

$(libDirectory):
	mkdir ../ExampleSrcRepo/lib

clean:
	rm ../ExampleSrcRepo/lib -r
	rm ../dist/GregBuild.exe
	rm ../ExampleSrcRepo/GregBuild.exe