include Makefile.inc

KERNEL=kernel
SOURCES=$(shell find . -name "*.c")
SOURCES_ASM=$(shell find . -name "*.asm" ! -name "loader.asm")
BUILDDIR=BUILD
OBJECTS=$(SOURCES:%.c=$(BUILDDIR)/%.o)
OBJECTS_ASM=$(SOURCES_ASM:%.asm=$(BUILDDIR)/%.asm.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:%.asm=$(BUILDDIR)/%.asm.o)
STATICLIBS=

all: $(KERNEL).bin

$(KERNEL).bin: $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL).bin $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o $(KERNEL).elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(GCC) $(GCCFLAGS) -I./include -I../Constants -c $< -o $@

$(BUILDDIR)/%.asm.o : %.asm
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf BUILD .bin

.PHONY: all clean
