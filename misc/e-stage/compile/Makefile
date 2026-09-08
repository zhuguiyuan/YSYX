TARGETS += a.tokens.txt a.ast.txt b.lint.txt
TARGETS += a.ll a.opt.ll a.volatile.opt.ll
TARGETS += a.x86.s a.rv64.s a.rv64.O1.s
TARGETS += a.x86.gcc.s a.rv64.gcc.s a.rv64.O1.gcc.s
all: $(TARGETS)

a.tokens.txt: a.c
	clang -fsyntax-only -Xclang -dump-tokens a.c > a.tokens.txt 2>&1

a.ast.txt: a.c
	clang -fsyntax-only -Xclang -ast-dump a.c > a.ast.txt 2>&1

a.ll: a.c
	clang -S -emit-llvm a.c -o a.ll

a.opt.ll: a.c
	clang -S -emit-llvm -O1 a.c -o a.opt.ll

a.volatile.opt.ll: a.volatile.c
	clang -S -emit-llvm -O1 a.volatile.c -o a.volatile.opt.ll

b.lint.txt:
	clang b.c --analyze -Xanalyzer -analyzer-output=text > b.lint.txt 2>&1

a.x86.s: a.c
	clang -S a.c -o a.x86.s

a.rv64.s: a.c
	clang -S a.c --target=riscv64-linux-gnu -o a.rv64.s

a.rv64.O1.s: a.c
	clang -S a.c --target=riscv64-linux-gnu -O1 -o a.rv64.O1.s

a.x86.gcc.s: a.c
	gcc -S a.c -o a.x86.gcc.s

a.rv64.gcc.s: a.c
	riscv64-linux-gnu-gcc -S a.c -o a.rv64.gcc.s

a.rv64.O1.gcc.s: a.c
	riscv64-linux-gnu-gcc -S a.c -O1 -o a.rv64.O1.gcc.s

clean:
	rm -f $(TARGETS)

.PHONY: all clean
