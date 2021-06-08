include ../Makefile.inc

MODULE=0000-sampleCodeModule
SOURCES=$(wildcard [^_]*.c)

all: $(MODULE).bin

$(MODULE).bin: $(SOURCES)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c $(SOURCES) -o ../$(MODULE).bin
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) -o ../$(MODULE).elf

clean:
	rm -rf *.o

.PHONY: all clean print
