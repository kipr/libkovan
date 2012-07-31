#include <kovan/kovan.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;
	a_button_set_text("Exit");
	
	while(!a_button()) {
		printf("Update %d\n", i++);
		for(size_t i = 0; i < 8; ++i) {
			printf("Back EMF (%d) = %d\n", i, get_backemf(i));
		}
		fflush(stdout);
		msleep(100);
		console_clear();
	}
	
	return 0;
}