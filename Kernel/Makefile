include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(shell find . -name "*.c")
SOURCES_ASM=$(shell find . -name "*.asm" ! -name "loader.asm")
BUILDDIR=BUILD
OBJECTS=$(SOURCES:%.c=$(BUILDDIR)/%.o)
OBJECTS_ASM=$(SOURCES_ASM:%.asm=$(BUILDDIR)/%.asm.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:%.asm=$(BUILDDIR)/%.asm.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o $@.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	objcopy -O binary $@.elf $@

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(GCC) $(GCCFLAGS) -I./include -I../Constants -I../Userland/libc -c $< -o $@

$(BUILDDIR)/%.asm.o : %.asm
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf BUILD .bin

.PHONY: all clean
