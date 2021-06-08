include Makefile.inc

# DATAFILES=0001-sampleDataModule.bin
PROGRAMS=SampleCodeModule
OUTDIR  = BUILD
OUTPUTS = $(PROGRAMS:%=$(OUTDIR)/%.bin)

all: libc $(OUTPUTS)

# sampleDataModule:
# 	printf "This is sample data." > $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)
$(OUTDIR)/%.bin:
	$(MAKE) -C $* -f ../ProgramMakefile.mk all

clean:
	rm -rf $(OUTDIR)
	cd libc; make clean

libc:
	cd libc; make libc.a

.PHONY: libc all clean
