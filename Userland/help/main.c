
#include "help.h"
#include <syscall.h>

int main() {
	write(1, help_txt, help_txt_len);
	return 0;
}