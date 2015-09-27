all: mbr.o
	ld ./bin/mbr.o -Ttext 0x7c00 --oformat=binary -m elf_i386 -o ./hello
mbr.o:
	as --32 -c ./src/hello.s -o ./bin/mbr.o
start: all
	qemu-system-i386 -hda ./hello
clean:
	rm ./bin/*.o hello
.PHONY: all clean start 
