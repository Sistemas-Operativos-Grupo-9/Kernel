# include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(shell find . -name "*.c")
SOURCES_ASM=$(shell find . -name "*.asm" ! -name "loader.asm")
BUILDDIR=BUILD
OBJECTS=$(SOURCES:%.c=$(BUILDDIR)/%.o)
OBJECTS_ASM=$(SOURCES_ASM:%.asm=$(BUILDDIR)/%.asm.o)
LOADERSRC=loader.asm


ASM = nasm
GCC = gcc
LD = gcc

ASMFLAGS=-felf64
CFLAGS=-m64 -fno-exceptions -fno-asynchronous-unwind-tables \
		  -fno-pie -fno-pic -mno-mmx -fno-builtin-malloc \
		  -fno-builtin-free -fno-builtin-realloc -mno-red-zone -fno-stack-check \
		  -fno-stack-protector -Wall -ffreestanding -nostdlib -fno-common -std=c99 \
		  -g -flto -O3 -march=nehalem -fno-isolate-erroneous-paths-dereference
LDFLAGS=-Wl,--warn-common,--oformat=elf64-x86-64,--build-id=none -static
# LDFLAGS=--warn-common -z max-page-size=0x1000 -T kernel.ld --oformat=elf64-x86-64 -nostdlib -g

LOADEROBJECT=$(LOADERSRC:%.asm=$(BUILDDIR)/%.asm.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(CFLAGS) $(LDFLAGS) -T kernel.ld -o $@.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	objcopy -O binary $@.elf $@

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(GCC) $(CFLAGS) -I./include -I../Constants -I../Userland/libc -c $< -o $@

$(BUILDDIR)/%.asm.o : %.asm
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@ -g

$(LOADEROBJECT):
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT) -g

clean:
	rm -rf BUILD .bin

.PHONY: all clean
