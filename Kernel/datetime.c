
#include "port.h"
#include <datetime.h>
#include <myUtils.h>

enum TimeVariable {
	SECONDS = 0,
	MINUTES = 2,
	HOURS = 4,
	DAY = 7,
	MONTH = 8,
	YEAR = 9,
	CENTRY = 0x32
};

uint8_t BCDToDecimal(uint8_t bcd) {
	return (bcd % 0xF0 >> 4) * 10 + (bcd & 0x0F);
}

uint8_t getAttrib(enum TimeVariable var) {
	out(0x70, var);

	return BCDToDecimal(in(0x71));
}

Time getTime() {
	return (Time){
	    getAttrib(SECONDS), getAttrib(MINUTES),
	    getAttrib(HOURS),   getAttrib(DAY),
	    getAttrib(MONTH),   getAttrib(YEAR) + getAttrib(CENTRY) * 100};
}