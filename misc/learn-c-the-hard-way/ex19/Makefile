CFLAGS=-Wall -g

all: ex19

ex19: object.o game.o

ex19p: object.o game.o

clean:
	rm -f ex19 ex19p object.o game.o

test: ex19
	./test.sh

.PHONY: test clean all
