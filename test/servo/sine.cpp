#include <kovan/kovan.hpp>
#include <cmath>
#include <sys/time.h>
#include <cstdio>

#define HALF_PI 1.57

int main(int argc, char *argv[])
{
	Servo servos[4] = {
		Servo(1),
		Servo(2),
		Servo(3),
		Servo(4)
	};
	
	for(unsigned char i = 0; i < 4; ++i) servos[i].setPosition(512);
	
	struct timeval tv;
	for(;;) {
		gettimeofday(&tv, NULL);
		float secs = (tv.tv_sec * 1000 + tv.tv_usec / 1000) / 1000.0f;
		printf("secs: %f", secs);
		unsigned int ticks = sin(secs) * 400 + 512;
		printf("\ttick: %u\n", ticks);
		for(unsigned char i = 0; i < 4; ++i) servos[i].setPosition(ticks);
		publish();
		printf("pos 0: %d\n", servos[0].position());
	}
}