#include <kovan/kovan.h>
#include <cmath>
#include <sys/time.h>
#include <stdio.h>

#define HALF_PI 1.57

int main(int argc, char *argv[])
{
	for(unsigned char i = 0; i < 4; ++i) set_servo_position(i, 512);
	
	struct timeval tv;
	for(;;) {
		gettimeofday(&tv, NULL);
		float secs = (tv.tv_sec * 1000 + tv.tv_usec / 1000) / 1000.0f;
		printf("secs: %f", secs);
		unsigned int ticks = sin(secs) * 400 + 512;
		printf("\ttick: %u\n", ticks);
		for(unsigned char i = 0; i < 4; ++i) set_servo_position(i, ticks);
		publish();
		printf("pos 0: %d\n", get_servo_position(0));
	}
}