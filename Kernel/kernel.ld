OUTPUT_FORMAT("binary")
ENTRY(loader)
SECTIONS
{
	.text 0x100000 :
	{
		text = .;
		*(.text*)
		. = ALIGN(0x1000);
		rodata = .;
		*(.rodata*)
	}
	.data ALIGN(0x1000) : AT(ADDR(.data))
	{
		data = .;
		*(.data*)
		endOfKernelBinary = .;
	}
	startOfModules = .;
	. += 0x200000-1;
	. = ALIGN(0x100000);
	endOfModules = .;
	sharedMemory = .;
	phyloMemory = .;
	. += 512;
	testsMemory = .;
	. += 512;
	.bss ALIGN(0x1000) : AT(ADDR(.bss))
	{
		bss = .;
		*(.bss*)
		*(EXCLUDE_FILE (*.o) COMMON)
	}
	. = ALIGN(0x1000);
	endOfKernel = .;
	. += 0x8000;
	. = ALIGN(0x100000);
	endOfKernelStack = .;
	. += 0x200000 * 256;
	startOfHeap = .;
}
