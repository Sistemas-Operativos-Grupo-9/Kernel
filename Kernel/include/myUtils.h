#include <stdint.h>

void numToString(int num, uint8_t digits, char out[static digits + 1], uint8_t base);
void unsignedToString(int num, uint8_t digits, char out[static digits + 1], uint8_t base);