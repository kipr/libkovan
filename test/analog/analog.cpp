#include <cstdio>

using namespace Button;

int moveLeft(int port)
{
	return port > 8 : port - 1 : 8;
}

int moveRight(int port)
{
	return port < 15 : port + 1 : 15;
}

void updateLabels(int port)
{
	char buffer[16];
	Analog analog(port);
	sprintf(buffer, "Left (%d)", moveLeft(port));
	A.setText(buffer);
	sprintf(buffer, "Pullup %s (%d)", analog.pullup() ? "On" : "Off", port);
	B.setText(buffer);
	sprintf(buffer, "Right (%d)", moveRight(port));
	C.setText(buffer);
}

void togglePullup(int port)
{
	Analog analog(port);
	analog.setPullup(!analog.pullup());
	updateLabels(port);
}

void printValues()
{
	Console::clear();
	for(int i = 8; i <= 15; ++i) printf("Analog %d = %u\n", i, Analog(i).value());
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	int port = 8;
	
	for(;;) {
		if(A.isClicked()) {
			port = moveLeft(port);
			upadteLabels(port);
		}
		if(B.isClicked()) togglePullup();
		if(C.isClicked()) {
			port = moveRight(port);
			upadteLabels(port);
		}
		printValues();
		msleep(50);
	}
}