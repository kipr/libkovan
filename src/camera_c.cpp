#include "kovan/camera.h"
#include "kovan/camera.hpp"
#include "nyi.h"
#include <iostream>

class DeviceSingleton
{
public:
	static Camera::Device *instance()
	{
		static Camera::Device s_device;
		return &s_device;
	}
};

int camera_open()
{
	return DeviceSingleton::instance()->open() ? 1 : 0;
}

int camera_open_device(int number)
{
	return DeviceSingleton::instance()->open(number) ? 1 : 0;
}

int camera_load_config(const char *name)
{
	Config *config = Config::load(Camera::ConfigPath::path(name));
	if(!config) return 0;
	DeviceSingleton::instance()->setConfig(*config);
	delete config;
	return 1;
}

void set_camera_width(int width)
{
	if(width <= 0) {
		std::cout << "Camera width must be greater than 0." << std::endl;
		return;
	}
	DeviceSingleton::instance()->setWidth(width);
}

void set_camera_height(int height)
{
	if(height <= 0) {
		std::cout << "Camera height must be greater than 0." << std::endl;
		return;
	}
	DeviceSingleton::instance()->setHeight(height);
}

int camera_update()
{
	return DeviceSingleton::instance()->update() ? 1 : 0;
}

pixel get_camera_pixel(point2 p)
{
	nyi("get_camera_pixel");
}

int get_channel_count()
{
	return DeviceSingleton::instance()->channels().size();
}

bool check_channel(int i)
{
	const Camera::ChannelPtrVector &channels = DeviceSingleton::instance()->channels();
	if(i < 0 || i >= channels.size()) {
		std::cout << "Channel must be in the range 0 .. " << (channels.size() - 1) << std::endl;
		return false;
	}
	return true;
}

bool check_channel_and_object(int i, int j)
{
	const Camera::ChannelPtrVector &channels = DeviceSingleton::instance()->channels();
	if(i < 0 || i >= channels.size()) {
		if(channels.size() < 1) std::cout << "Active configuration doesn't have any channels.";
		else std::cout << "Channel must be in the range 0 .. " << (channels.size() - 1);
		std::cout << std::endl;
		return false;
	}
	const Camera::ObjectVector *objs = channels[i]->objects();
	if(j < 0 || j >= objs->size()) {
		std::cout << "No such object " << j << std::endl;
		return false;
	}
	return true;
}

int get_object_count(int channel)
{
	if(!check_channel(channel)) return -1;
	return DeviceSingleton::instance()->channels()[channel]->objects()->size();
}

double get_object_confidence(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return 0.0;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.confidence();
}


const char *get_object_data(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return 0;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.data();
}

int get_object_data_length(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return 0;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.dataLength();
}

int get_object_area(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return -1;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.boundingBox().area();
}

rectangle get_object_bbox(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return create_rectangle(-1, -1, 0, 0);
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.boundingBox().toCRectangle();
}

point2 get_object_centroid(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return create_point2(-1, -1);
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.centroid().toCPoint2();
}

void camera_close()
{
	DeviceSingleton::instance()->close();
}