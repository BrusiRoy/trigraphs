.PHONY=clean

compile:
	clang++ trigraphs.cpp -O2 -Wextra -Wall -std=c++1y -o trigraphs

clean:
	rm trigraphs
