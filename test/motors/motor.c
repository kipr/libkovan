#include <kovan/kovan.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("Starting Motor\n");
	motor(0, 100);
	motor(2, 100);
	for(;;) {}
	printf("All Off\n");
	return 0;
}