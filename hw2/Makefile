AC=as
ACFLAGS=--32
CC=gcc
CCFLAGS=-ffreestanding -m32
LD=ld
LDFLAGS=-m elf_i386
BIN=./bin/
SRC=./src/
ASM=./src/asm/

OBJ= \
	$(ASM)boot.S.o        \
	$(ASM)dt_loaders.S.o  \
	$(ASM)ints.S.o        \
	$(BIN)desc_tables.c.o \
	$(BIN)gdt.c.o         \
	$(BIN)helpers.c.o     \
	$(BIN)idt.c.o         \
	$(BIN)isr.c.o         \
	$(BIN)pic.c.o         \
	$(BIN)screen.c.o      \
	$(BIN)main.c.o   

all: build

bin:
	mkdir -p ./bin

build: $(OBJ)
	$(LD) $(LDFLAGS) -T linker.ld $(OBJ) -o boot

$(ASM)%.S.o: $(ASM)%.S
	$(AC) $(ACFLAGS) -c $< -o $@

$(BIN)%.c.o: $(SRC)%.c
	$(CC) $(CCFLAGS) -c $< -o $@

start: bin build
	qemu-system-i386 -kernel boot

debug:
	qemu-system-i386 -kernel kernel -d int

monitor:
	qemu-system-i386 -kernel kernel -monitor stdio

clean:
	rm ./bin/*.o boot

.PHONY: build clean start
