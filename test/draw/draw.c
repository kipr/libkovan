#include <kovan/kovan.h>

int main(int argc, char *argv[])
{
	draw_open();
	draw_line(0, 0, 200, 200);
	// sleep(5.0f);
	draw_close();
}