
PROGRAM = $(shell basename "$(CURDIR)")
BUILDDIR = ../BUILD
OUTPUT = $(BUILDDIR)/$(PROGRAM).bin
SOURCES = ../_loader.c $(shell find . -name "*.c")
OBJS = $(SOURCES:%.c=$(BUILDDIR)/$(PROGRAM)/%.o)

CFLAGS = -I../libc -I../../Constants -m64 -fno-exceptions -std=c99 \
		 -Wall -ffreestanding -nostdlib -fno-common -mno-red-zone \
		 -mno-mmx -fno-builtin-malloc -fno-builtin-free \
		 -fno-builtin-realloc -fno-stack-check -fno-stack-protector \
		 -g -fpie -fpic -fdata-sections -ffunction-sections -march=nehalem -flto -Werror # -Os -flto
LDFLAGS = -Wl,--warn-common,--build-id=none,-pie,--oformat=elf64-x86-64,--gc-sections -static
LDLIBS = -L../libc

LDVER = $(shell ld -v | tr ' ' '\n' | tail -1)
$(info LD version: $(LDVER))

ifneq "$(LDVER)" "2.25"
$(info LD version is not docker, adding --no-dynamic-linker flag)
LDFLAGS += -Wl,--no-dynamic-linker
endif


LD=gcc
GCC=gcc

# $(info $(OBJS))

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
