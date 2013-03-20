#include "kovan/kovan.hpp"
#include <cstdlib>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
	ARDrone *drone = ARDrone::instance();
	if(!drone->connect()) {
		return EXIT_FAILURE;
	}
	drone->setActiveCamera(ARDrone::Front);
	drone->flatTrim();
	msleep(1000);
	
	std::cout << "flying? " << (drone->state() == ARDrone::Flying) << std::endl;
	
	// drone->takeoff();
	
	const double start = seconds();
	while(seconds() - start < 20.0) {
		cv::Mat m = drone->rawImage();
		std::cout << m.size().width << " x " << m.size().height << std::endl;
		cv::waitKey(1);
		msleep(30);
	}
	
	drone->land();
	msleep(2000);

	drone->disconnect();
	
	return EXIT_SUCCESS;
}