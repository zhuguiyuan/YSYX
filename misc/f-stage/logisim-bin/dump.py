#!/bin/env python3

import sys
from pathlib import Path
from subprocess import check_call


def usage():
    prog = Path(sys.argv[0]).absolute()
    print(f"usage: {prog.name} <file.s>")
    sys.exit(-1)


if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] in ["-h", "--help"]:
        usage()

    asm_file = Path(sys.argv[1])
    if not asm_file.exists() or not asm_file.is_file():
        print(f'error: "{asm_file}" doesn\'t exists or is not file')
        usage()

    obj_file = asm_file.with_suffix(".o")
    elf_file = asm_file.with_suffix(".elf")
    bin_file = asm_file.with_suffix(".bin")
    as_cmd = "llvm-mc --arch=riscv32 -filetype=obj"
    ld_cmd = "ld.lld -m elf32lriscv -Ttext 0 -e _start"
    dump_cmd = "llvm-objdump -d"
    copy_cmd = "llvm-objcopy -O binary"
    check_call(f"{as_cmd} {asm_file} -o {obj_file}".split())
    check_call(f"{ld_cmd} {obj_file} -o {elf_file}".split())
    check_call(f"{dump_cmd} {elf_file}".split())
    check_call(f"{copy_cmd} {elf_file} {bin_file}".split())
    check_call(f"rm -f {obj_file} {elf_file}".split())
