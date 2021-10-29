#target remote localhost:1234
add-symbol-file Kernel/kernel.bin.elf 0x100000
add-symbol-file Userland/BUILD/shell.bin.elf 0x600000
add-symbol-file Userland/BUILD/shell.bin.elf 0x700000
set disassembly-flavor intel
directory Kernel
directory Kernel/interrupts
layout src
#b 0x100000