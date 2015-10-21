cc = g++
flags = -Wall -Werror -ansi -pedantic

all: rshell

rshell: bin
	$(cc) $(flags) src/rshell.cpp -o bin/rshell

bin:
	mkdir bin

clean:
	rm -rf bin
