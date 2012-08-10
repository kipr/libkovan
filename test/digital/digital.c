#include <stdio.h>

int main()
{
	for(;;) {
		int i = 0;
		for(; i < 8; ++i) printf("digital %d = %d\n", i, digital(i));
		console_clear();
		msleep(100);
	}
	return 0;
}