#include <stdio.h>

int main()
{
	int state = 0;
	for(;;) {
		int i = 0;
		for(; i < 8; ++i) set_digital_pullup(i, state);
		char dummy[1];
		gets(dummy);
		
		state = !state;
	}
	return 0;
}