
CFLAGS = -m64 -std=c99 -Wall -fno-exceptions -ffreestanding -nostdlib \
		 -fno-common -mno-red-zone -mno-mmx -fno-builtin-malloc \
		 -fno-builtin-free -fno-builtin-realloc -fno-stack-check \
		 -fno-stack-protector -fPIC -g
AR = ar
GCC = gcc

SOURCES = $(shell find -L . -name "*.c")
OBJDIR = BUILD
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

$(info $(SOURCES))
libc.a: $(OBJECTS)
	$(AR) rcs -o $@ $^

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(GCC) $(CFLAGS) -I"../../Memory-Manager/include" -I"../../Constants" -c -o $@ $^

clean:
	rm -rf -- libc.a BUILD

.PHONY: libc.a clean
