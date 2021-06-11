
#include "stdint.h"
#include "time.h"
#include "myUtils.h"

void wait() {
    for (uint64_t i = 0; i < 40000000UL; i++) __asm__("nop");
}


void toISO8601(Time time, char out[21]) {
    numToString(time.year, 4, out, 10);
    out[4] = '-';
    numToString(time.month, 2, out + 5, 10);
    out[7] = '-';
    numToString(time.day, 2, out + 8, 10);
    out[10] = 'T';
    numToString(time.hours, 2, out + 11, 10);
    out[13] = ':';
    numToString(time.minutes, 2, out + 14, 10);
    out[16] = ':';
    numToString(time.seconds, 2, out + 17, 10);
    out[19] = 'Z';
    out[20] = '\0';
}