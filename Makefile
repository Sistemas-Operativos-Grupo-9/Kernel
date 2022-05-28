
all: bootloader memory_manager kernel userland image

bootloader:
	cd Bootloader; make all

memory_manager:
	cd Memory-Manager; make

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

toolchain:
	cd Toolchain; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Memory-Manager; make clean
	cd Userland; make clean

rebuild: clean all

.PHONY: toolchain bootloader image collections kernel userland all clean
