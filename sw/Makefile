distDirectory = ../dist
libDirectory = "../ExampleSrcRepo/lib"

GregBuild: $(distDirectory) GregTestDll
	# Compile 
	gcc core/GregBuildMain.c fileSystemRecursion/FileAndTestCaseGatherer.c testMainWriting/TestMainWriter.c fileSystemRecursion/TestAndSrcDefinitions.c -o ../dist/GregBuild.exe
	
	cp ../dist/GregBuild.exe ../ExampleSrcRepo

GregTestDll: $(libDirectory)
	cp external/GregTest/dist/GregTest.dll ../ExampleSrcRepo/lib/GregTest.dll

$(distDirectory):
	mkdir ../dist

$(libDirectory):
	mkdir ../ExampleSrcRepo/lib

clean:
	rm ../dist/GregBuild.exe
	rm ../ExampleSrcRepo/GregBuild.exe