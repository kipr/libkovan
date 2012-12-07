#include "channel_p.hpp"
#include "warn.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace Private::Camera;

UnknownChannel::UnknownChannel(::Camera::Device *device, const Config &config)
	: ::Camera::Channel(device, config)
{
	WARN("Creating unknown channel");
}

void UnknownChannel::update()
{
	
}

ColorChannel::ColorChannel(::Camera::Device *device, const Config &config)
	: ::Camera::Channel(device, config)
{
	if(!config.containsKey("bottom_h")) WARN("Did not find bottom_h key in config.");
	int bh = config.intValue("bottom_h");
	
	if(!config.containsKey("bottom_s")) WARN("Did not find bottom_s key in config.");
	int bs = config.intValue("bottom_s");
	
	if(!config.containsKey("bottom_v")) WARN("Did not find bottom_v key in config.");
	int bv = config.intValue("bottom_v");
	
	if(!config.containsKey("top_h")) WARN("Did not find top_h key in config.");
	int th = config.intValue("top_h");
	
	if(!config.containsKey("top_s")) WARN("Did not find top_s key in config.");
	int ts = config.intValue("top_s");
	
	if(!config.containsKey("top_v")) WARN("Did not find top_v key in config.");
	int tv = config.intValue("top_v");
	
	m_bottom = cv::Scalar(bh, bs, bv);
	m_top = cv::Scalar(th, ts, tv);
}

void ColorChannel::update()
{
	
}