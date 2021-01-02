distDirectory = ../dist

GregTestMain: $(distDirectory)
	gcc GregTestMain.c TestGatherer/TestGatherer.c TestGatherer/TestMainWriter.c TestGatherer/TestAndSrcDefinitions.c -o ../dist/GregTest.exe
	
	cp ../dist/GregTest.exe ../ExampleSrcRepo

$(distDirectory):
	mkdir ../dist

clean:
	rm ../dist/GregTest.exe
	rm ../ExampleSrcRepo/GregTest.exe