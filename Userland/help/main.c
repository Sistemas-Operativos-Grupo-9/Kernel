
#include "help.h"
#include <syscall.h>

int main() {
	write(1, (const char *)compiled_txt, compiled_txt_len);
	return 0;
}
