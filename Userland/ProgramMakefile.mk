
PROGRAM = $(shell basename "$(CURDIR)")
BUILDDIR = ../BUILD
OUTPUT = $(BUILDDIR)/$(PROGRAM).bin
SOURCES = ../_loader.c $(shell find . -name "*.c")
OBJS = $(SOURCES:%.c=$(BUILDDIR)/$(PROGRAM)/%.o)

CFLAGS = -I../libc -I../../Constants -m64 -fno-exceptions -std=c99 -Wall -ffreestanding -nostdlib -fno-common -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -g -fpie
LDFLAGS = -Wl,-pie,--oformat=elf64-x86-64 
LDLIBS = -L../libc

LD=gcc
GCC=gcc

$(info $(OBJS))

$(BUILDDIR)/$(PROGRAM)/%.o: %.c
	mkdir -p $(BUILDDIR)/$(PROGRAM)
	$(GCC) $(CFLAGS) -c -o $@ $^

$(BUILDDIR)/%.bin: $(OBJS)
	mkdir -p $(BUILDDIR)
	$(LD) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -T ../linkscript.ld -o $@.elf $^ -l:libc.a
	objcopy -O binary $@.elf $@


all: $(OUTPUT)

.PHONY: all
.PRECIOUS: $(BUILDDIR)/$(PROGRAM)/%.o