#include <kovan/kovan.h>

int main(int argc, char *argv[])
{
	while(!a_button_pressed());
	printf("%d\n", a_button());
}