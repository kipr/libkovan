#include <kovan/kovan.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	const static unsigned long diff = 12341UL;
	const double start = seconds();
	msleep(diff);
	const double end = seconds();
	printf("Expected diff = %lu, actual diff = %lu\n", diff, (unsigned long)((end - start) * 1000.0));
	
	return 0;
}