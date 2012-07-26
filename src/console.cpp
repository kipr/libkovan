#include "console.hpp"
#include <cstdio>

void Console::clear()
{
	printf("\f");
	fflush(stdout);
}