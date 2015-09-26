all: mbr.o
	ld ./bin/mbr.o -Ttext 0x7c00 --oformat=binary -m elf_i386 -o ./mbr
mbr.o:
	as --32 -c ./src/hello.s -o ./bin/mbr.o
start: all
	qemu-system-i386 -hda ./mbr
clear:
	rm ./bin/*.o mbr
.PHONY: all clear start 
