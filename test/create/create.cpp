#include <kovan.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	Create *create = Create::instance();
	if(!create->connect()) {
		cout << "Create connection failed" << endl;
		return 1;
	}

	create->setFullMode();
	Sensor<bool> *digitalInput0 = create->cargoBayDigitalInput(0);
	while(!digitalInput0->value());

	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 4; ++j) {
			create->move(500, 200);
			msleep(1000);
			create->turn(90, 400);
			msleep(1000);
		}
	}

	create->disconnect();

	return 0;
}