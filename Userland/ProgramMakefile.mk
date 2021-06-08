
PROGRAM = $(shell basename "$(CURDIR)")
BUILDDIR = ../BUILD
OUTPUT = $(BUILDDIR)/$(PROGRAM).bin
SOURCES = ../_loader.c $(shell find . -name "*.c")
OBJS = $(SOURCES:%.c=$(BUILDDIR)/$(PROGRAM)/%.o)

CFLAGS = -I../libc -I../../Constants -m64 -fno-exceptions -std=c99 -Wall -ffreestanding -nostdlib -fno-common -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -g
LDLIBS = -L../libc

LD = ld
GCC=gcc

$(info $(OBJS))

$(BUILDDIR)/$(PROGRAM)/%.o: %.c
	mkdir -p $(BUILDDIR)/$(PROGRAM)
	$(GCC) $(CFLAGS) -c -o $@ $^

$(BUILDDIR)/%.bin: $(OBJS)
	mkdir -p $(BUILDDIR)
	$(LD) $(LDLIBS) -T ../linkscript.ld -o $@ $^ -l:libc.a
	$(LD) $(LDLIBS) -T ../linkscript.ld --oformat=elf64-x86-64 -o $@.elf $^ -l:libc.a

all: $(OUTPUT)

.PHONY: all
.PRECIOUS: $(BUILDDIR)/$(PROGRAM)/%.o