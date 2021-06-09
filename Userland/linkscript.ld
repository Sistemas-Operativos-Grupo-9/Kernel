OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS
{
	.text :
	{
		*(.text*)
		. = ALIGN(0x1000);
		*(.rodata*)
	}
	.data ALIGN(0x1000) :
	{
		*(.data*)
	}
	.bss ALIGN(0x1000) :
	{
		bss = .;
		*(.bss*)
	}
	endOfBinary = .;
}