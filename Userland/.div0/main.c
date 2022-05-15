
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
int main() {
	volatile int x = 0;
	return 1 / x;
}