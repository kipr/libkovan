#include <kovan/kovan.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("Starting Motor\n");
	motor(0, 50);
	msleep(4000);
	printf("All Off\n");
	ao();
	return 0;
}