#include "kovan/debug.h"
#include "kovan_p.hpp"

#include <cstdio>
#include <iostream>

void debug_print_registers()
{
	const unsigned short *regs = Private::Kovan::instance()->currentState().t;
	std::cout << "[";
	for(int i = 0; i < TOTAL_REGS; ++i) {
		std::cout << i << ": " << regs[i];
		if(i < TOTAL_REGS - 1) {
			std::cout << ", ";
			if(i % 4 == 0 && i != 0) std::cout << std::endl;
		}
	}
	std::cout << "]" << std::endl;
}

unsigned short register_value(unsigned short addy)
{
	if(addy >= TOTAL_REGS) return 0xFFFF;
	return Private::Kovan::instance()->currentState().t[addy];
}

int debug_dump_data(const unsigned char *const data, const size_t size, const char *const where)
{
	FILE *fp = fopen(where, "w");
	if(!fp) return 0;
	size_t ret = fwrite(data, 1, size, fp);
	fclose(fp);
	return ret ? 1 : 0;
}