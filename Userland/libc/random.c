static unsigned int seed = 1;
void srand(int newseed) { seed = (unsigned)newseed & 0x7fffffffU; }
int rand(void) {
	seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
	return (int)seed;
}