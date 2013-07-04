#include <kovan/kovan.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if(!drone_connect()) {
		printf("Unable to connect to drone!");
		return 1;
	}
	
	if(get_drone_version() != 2) {
		printf("This program needs to be run on a version 2 drone!\n");
		return 1;
	}
	
	printf("Connected to a version 2 drone!\n");
	
	drone_calibrate();
	
	drone_takeoff();
	msleep(3000);
	
	drone_move(.3f, .0f, .0f, .0f);
	msleep(3000);
	
	
	
	drone_land();
	msleep(3000);
	
	printf("FINAL X: %f\n", get_drone_x());
	
	drone_disconnect();
	
	return 0;
}