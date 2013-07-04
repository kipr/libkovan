#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include <sstream>

#include <kovan/kovan.hpp>

#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	
	ARDrone *drone = ARDrone::instance();
	if(!drone->connect()) {
		std::cout << "Failed to connect to drone" << std::endl;
		return 1;
	}
	
	Camera::Device device(new Camera::ARDroneInputProvider);
	if(!device.open()) return 1;
	
	//drone->setActiveCamera(ARDrone::Bottom);
	
	// device.setWidth(320);
	// device.setHeight(240);
	cv::namedWindow("Blobs");

	Config config;
	config.setValue("camera/num_channels", 1);
	config.setValue("camera/channel_0/type", "qr");
	
	config.setValue("camera/channel_0/th", 75);
	config.setValue("camera/channel_0/ts", 200);
	config.setValue("camera/channel_0/tv", 225);
	
	config.setValue("camera/channel_0/bh", 51);
	config.setValue("camera/channel_0/bs", 55);
	config.setValue("camera/channel_0/bv", 85);
	
	device.setConfig(config);

	time_t start, end;
	unsigned long counter = 0;
	double sec;

	time(&start);

	drone->takeoff();
	msleep(4000);

	cv::Mat image;
	double yaw = 0.0;
	while(cv::waitKey(1) == -1) {
		device.update();
		image = device.rawImage();
		if(image.empty()) continue;
		const Camera::ObjectVector *objects = device.channels()[0]->objects();
		if(!objects) continue;
		Camera::ObjectVector::const_iterator it = objects->begin();
		for(; it != objects->end(); ++it) {
			const Camera::Object &object = *it;
			cv::rectangle(image, cv::Rect(object.boundingBox().x(), object.boundingBox().y(),
				object.boundingBox().width(), object.boundingBox().height()), cv::Scalar(0, 255, 0));
		}
		
		if(objects->size() > 0) {
			Camera::Object obj = objects->at(0);
			std::cout << obj.data() << std::endl;
			int offset = obj.boundingBox().center().x() - image.size().width / 2;
			std::cout << offset / (image.size().width / 2.0) * 0.3f << std::endl;
			yaw = yaw*0.9 + offset*0.05*0.1;
		} else {
			yaw = yaw*0.9;
		}
		drone->move(0.0f, 0.0f, 0.0f, yaw);
		
		time(&end);

		++counter;
		sec = difftime(end, start);      

		double fps = counter / sec;
		
		char buffer[12];
		sprintf(buffer, "%2.f", fps);
		cv::putText(image, buffer, cv::Point(5, 15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0));
		
		cv::imshow("Blobs", image);
	}
	
	drone->land();
	msleep(2000);
	
	
	device.close();
	drone->disconnect();
	
	cv::destroyAllWindows();
}