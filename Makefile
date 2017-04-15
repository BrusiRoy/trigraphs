.PHONY=clean

compile:
	clang++ trigraphs.cpp -O2 -Wextra -Wall -std=c++1y -o trigraphs.o

run: compile
	./trigraphs.o sampleInput.cpp sampleOutput.cpp

runSample: run
	clang++ sampleOutput.cpp -trigraphs -o sampleOutput.o
	./sampleOutput.o

clean:
	rm *.o
