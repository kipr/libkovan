#include <kovan.hpp>
#include <string>
#include <iostream>

using namespace std;

void continueMessage(const std::string& message)
{
	cout << message << endl;
	cout << "Press Play to continue..." << endl;
	Create::instance()->playButton()->waitUntilClicked();
}

void stopWhen(const Sensor<bool> *sensor)
{
	while(!sensor->value());
	Create::instance()->stop();
}

void moveTest(Create *create)
{
	continueMessage("Driving forward 1 meter");
	create->move(1000, 200);
}

void turnTest(Create *create)
{
	continueMessage("Turning 90 degrees");
	create->turn(90, 150);
}

void bumpTest(Create *create)
{
	continueMessage("Driving forward until bumped");
	create->driveDirect(200, 200);
	SensorLogic::Or sensor(create->bumpLeft(), create->bumpRight());
	stopWhen(&sensor);
}

int main(int argc, char *argv[])
{
	Create *create = Create::instance();
	if(!create->connect()) {
		cout << "Create connection failed" << endl;
		return 1;
	}

	continueMessage("iRobot (R) Create (TM) Test!");

	moveTest(create);
	turnTest(create);
	bumpTest(create);

	create->disconnect();

	return 0;
}