TARGETS = a.x86.pp.c a.rv64.pp.c a.include.txt a.x86.custom_pp.c
TARGETS += macros.x86.c macros.rv64.c macros.x86_rv64.diff

all: $(TARGETS)

a.x86.pp.c: a.c
	gcc -E a.c -o a.x86.pp.c

a.rv64.pp.c: a.c
	riscv64-linux-gnu-gcc -E a.c -o a.rv64.pp.c

a.include.txt: a.c
	gcc -E a.c --verbose > /dev/null 2> a.include.txt

a.x86.custom_pp.c: a.c stdio.h
	gcc -E -I. a.c -o a.x86.custom_pp.c

macros.x86.c:
	echo | gcc -dM -E - | sort > macros.x86.c

macros.rv64.c:
	echo | riscv64-linux-gnu-gcc -dM -E - | sort > macros.rv64.c

macros.x86_rv64.diff: macros.x86.c macros.rv64.c
	diff macros.x86.c macros.rv64.c > macros.x86_rv64.diff | true

clean:
	rm -f $(TARGETS)

.PHONY:all clean
