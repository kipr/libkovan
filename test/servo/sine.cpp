#include <kovan.hpp>
#include <cmath>
#include <sys/time.h>

#define HALF_PI 1.57

int main(int argc, char *argv[])
{
	Servo servos[4] = {
		Servo(1),
		Servo(2),
		Servo(3),
		Servo(4)
	};
	
	struct timeval tv;
	for(;;) {
		gettimeofday(&tv, NULL);
		float secs = (tv.tv_sec * 1000 + tv.tv_usec / 1000) / 1000.0f;
		for(unsigned char i = 0; i < 4; ++i) servos[i].setPosition(sin(secs + HALF_PI * i) * 400 + 512);
	}
}