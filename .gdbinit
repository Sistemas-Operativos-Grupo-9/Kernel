#target remote localhost:1234
add-symbol-file Kernel/kernel.bin.elf 0x100000
add-symbol-file Userland/BUILD/shell.bin.elf 0x500000
add-symbol-file Userland/BUILD/shell.bin.elf 0x600000
set disassembly-flavor intel
directory Kernel
directory Kernel/interrupts
layout src
#b 0x100000