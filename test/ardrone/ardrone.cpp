#include "kovan/kovan.hpp"
#include <cstdlib>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
	ARDrone *drone = ARDrone::instance();
	if(!drone->connect()) {
		std::cout << "Couldn't connect to the drone" << std::endl;
		return EXIT_FAILURE;
	}
	drone->setActiveCamera(ARDrone::Front);
	drone->flatTrim();
	if(drone->version() == ARDrone::V2) {
		std::cout << "Version 2 Drone!" << std::endl;
	} else {
		std::cout << "Version 1 Drone!" << std::endl;
	}
	// drone->pair();
	msleep(1000);
	
	// std::cout << "flying? " << (drone->state() == ARDrone::Flying) << std::endl;
	
	// drone->takeoff();
	
	std::cout << "Starting main loop" << std::endl;
	const double start = seconds();
	// drone->move(0.0, 0.0, 0.0, 0.2);
	for(;;) {
		cv::Mat m;
		drone->rawImage(m);
		msleep(30);
		// if(m.empty()) continue;
		cv::imshow("test", m);
		/* const ARDrone::NavigationData nd = drone->navigationData();
		std::cout << "roll: " << nd.roll << " pitch: " << nd.pitch << " yaw: " << nd.yaw << std::endl;
		std::cout << "altitude: " << nd.altitude << std::endl;
		std::cout << "vel: " << nd.velocity.x << ", " << nd.velocity.y << ", " << nd.velocity.z << std::endl; */
		// cv::imshow("Data", m);
		if(cv::waitKey(1) > 0) break;
	}
	
	drone->land();
	msleep(2000);

	drone->disconnect();
	
	return EXIT_SUCCESS;
}