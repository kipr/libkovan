#include <kovan/kovan.hpp>

int main(int argc, char *argv[])
{
	Camera::Device device;
	Config *config = Config::load("test.conf");
	device.setConfig(*config);
	delete config;
	
	return 0;
}