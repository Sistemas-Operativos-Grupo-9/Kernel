
# DATAFILES=0001-sampleDataModule.bin
PROGRAMS=$(shell find . -maxdepth 1 -mindepth 1 -type d ! -name BUILD ! -name libc)
OUTDIR  = BUILD
OUTPUTS = $(PROGRAMS:%=$(OUTDIR)/%.bin)

$(info $(OUTPUTS))

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
