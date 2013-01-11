#include "kovan/debug.h"
#include "kovan_p.hpp"

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