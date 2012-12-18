#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>

#include <kovan/kovan.hpp>

cv::Mat just;

void onMouse(int event, int x, int y, int, void*)
{
	if(event != CV_EVENT_LBUTTONDOWN) return;
	std::cout << just.cols << std::endl;
	std::cout << y << std::endl;
	std::cout << x << std::endl;
	cv::Vec3b s = just.at<cv::Vec3b>(y, x);
	std::cout << (int)s[0] << ", " << (int)s[1] << ", " << (int)s[2] << std::endl;
}

int main(int argc, char *argv[])
{
	Camera::Device device;
	device.open(1);
	cv::namedWindow("Blobs");

	Config config;
	config.setValue("camera/num_channels", 2);
	config.setValue("camera/channel_0/type", "hsv");
	
	config.setValue("camera/channel_0/th", 10);
	config.setValue("camera/channel_0/ts", 240);
	config.setValue("camera/channel_0/tv", 175);
	
	config.setValue("camera/channel_0/bh", 160);
	config.setValue("camera/channel_0/bs", 150);
	config.setValue("camera/channel_0/bv", 100);
	
	config.setValue("camera/channel_1/type", "barcode");
	
	device.setConfig(config);

	cv::setMouseCallback("Blobs", onMouse);

	cv::Mat image;
	while(cv::waitKey(1) == -1) {
		device.update();
		device.videoCapture()->retrieve(image);
		cv::cvtColor(image, just, CV_BGR2HSV);
		const Camera::ObjectVector &objects = device.channels()[1]->objects();
		Camera::ObjectVector::const_iterator it = objects.begin();
		for(; it != objects.end(); ++it) {
			const Camera::Object &object = *it;
			cv::rectangle(image, cv::Rect(object.boundingBox().x(), object.boundingBox().y(),
				object.boundingBox().width(), object.boundingBox().height()), cv::Scalar(0, 255, 0));
		}
		
		cv::imshow("Blobs", image);
	}
	
	device.close();
	
	cv::destroyAllWindows();
}