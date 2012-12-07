#include <kovan/kovan.h>
#include <math.h>

#define PI 3.14159

int main(int argc, char *argv[])
{
	double i = 0;
	for(; i < PI * 2; i += PI / 16) {
		datalog_append_number(i, "Input Angle");
		datalog_append_number(sin(i), "sin");
		datalog_append_number(cos(i), "cos");
		datalog_append_number(tan(i), "tan");
	}
	
	datalog_write_csv("test.csv");
	
	return 0;
}