#target remote localhost:1234
add-symbol-file Kernel/kernel.bin.elf 0x100000
#add-symbol-file Userland/BUILD/hangman.bin.elf 0x900000
add-symbol-file Userland/BUILD/shell.bin.elf 0xa00000
add-symbol-file Userland/BUILD/shell.bin.elf 0xc00000
add-symbol-file Userland/BUILD/date.bin.elf 0x1600000
set disassembly-flavor intel
directory Kernel
directory Kernel/interrupts
layout split
#b 0x100000