CPPFLAGS += $(shell pkg-config --cflags glib-2.0)
LDLIBS   += $(shell pkg-config --libs glib-2.0)

all: main

dev: CFLAGS += -g
dev: all

lexer.o: dbg.h lexer.h
main.o: dbg.h lexer.h
main: main.o lexer.o

clean:
	rm -f main lexer.o main.o

ide:
	bear -- $(MAKE)

.PHONY: all clean ide
