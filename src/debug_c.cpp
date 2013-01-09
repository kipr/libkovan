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