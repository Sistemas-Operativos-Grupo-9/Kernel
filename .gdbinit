target remote localhost:1234
add-symbol-file Kernel/kernel.bin.elf 0x100000
add-symbol-file Userland/BUILD/SampleCodeModule.bin.elf 0x400000
add-symbol-file Userland/BUILD/shell.bin.elf 0x500000
add-symbol-file Userland/BUILD/shell.bin.elf 0x600000
set disassembly-flavor intel
