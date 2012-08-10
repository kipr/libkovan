#include <stdio.h>

int main()
{
	int i = 0;
	set_auto_publishing(0);
	int state = 0;
	for(;;) {
		for(i = 0; i < 8; ++i) set_digital_output(i, 1);
		for(i = 0; i < 8; ++i) set_digital_value(i, state);
		state = !state;
		printf("Digital 0 val = %d\n", get_digital_value(0));
		printf("Digital 0 Output? = %d\n", get_digital_output(0));
		msleep(100);
		publish();
	}
	return 0;
}