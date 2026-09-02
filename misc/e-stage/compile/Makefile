TARGETS = a.tokens.txt
all: $(TARGETS)

a.tokens.txt: a.c
	clang -fsyntax-only -Xclang -dump-tokens a.c > a.tokens.txt 2>&1

clean:
	rm -f $(TARGETS)

.PHONY: all clean
