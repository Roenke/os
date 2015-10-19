OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
	. = 0x1000000 + SIZEOF_HEADERS;
	.text : { *(.text) }
}
