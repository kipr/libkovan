#include <kovan/kovan.hpp>
#include <string>
#include <iostream>

using namespace std;

void continueMessage(const std::string& message)
{
	cout << message << endl;
	cout << "Press Play to continue..." << endl;
	Create::instance()->advanceButton()->waitUntilClicked();
}

void stopWhen(const Sensor<bool> *sensor)
{
	while(!sensor->value()) {
		cout << "Sensor output: " << sensor->value() << endl;
		msleep(50);
	}
	cout << "Stopped!" << endl;
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

void cliffTest(Create *create)
{
	continueMessage("Driving forward until... cliffed?");
	create->driveDirect(200, 200);
	SensorLogic::Or sensor(create->cliffLeft(), create->cliffRight());
	stopWhen(&sensor);
}

void distanceTest(Create *create)
{
	continueMessage("Drive forward while reading dist");
	create->driveDirect(200, 200);
	SensorLogic::Or sensor(create->bumpLeft(), create->bumpRight());
	while(!sensor.value()) {
		cout << "value: " << create->distance()->value() << endl;
	}
}

int main(int argc, char *argv[])
{
	Create *create = Create::instance();
	if(!create->connect()) {
		cout << "Create connection failed" << endl;
		return 1;
	}

	create->setFullMode();

	create->setLeds(false, false, 255, 255);

	continueMessage("iRobot (R) Create (TM) Test!");

	distanceTest(create);
	moveTest(create);
	turnTest(create);
	bumpTest(create);
	cliffTest(create);

	create->disconnect();

	return 0;
}