OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
	. = 0x1000000 + SIZEOF_HEADERS;
	.text : { *(.text) }
    .rodata BLOCK(4K) : ALIGN (4K)
    {
        *(.rodata) 
    }
   .data  BLOCK (4K) : ALIGN (4K)
    {
        *(.data) 
    }
   .bss   BLOCK (4K) : ALIGN (4K)
    {
        *(.bss)  
    }
}
