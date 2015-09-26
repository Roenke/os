all: hello

hello: hello.o
	ld -m elf_i386 -T hello.ld ./bin/hello.o -o hello
hello.o:
	as -c --32 ./src/hello.s -o./bin/hello.o 
start: hello
	qemu-system-i386 -kernel hello
clear:
	rm ./bin/*.o hello

.PHONY: hello clear start