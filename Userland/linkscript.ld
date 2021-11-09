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
		got = .;
		*(.got*)
		. += 8 * 32;
		gotEnd = .;
		*(.data*)
	}
	.bss ALIGN(0x1000) :
	{
		bss = .;
		*(.bss*)
	}
	endOfBinary = .;
}