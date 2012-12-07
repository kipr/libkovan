#include "kovan/camera.hpp"
#include "channel_p.hpp"
#include "warn.hpp"

#include <opencv2/highgui/highgui.hpp>

using namespace Camera;

// Object //

Camera::Object::Object(const Point2<unsigned> &centroid,
	const Rectangle<unsigned> &boundingBox,
	const double &confidence, const char *data,
	const size_t &dataLength)
	: m_centroid(centroid),
	m_boundingBox(boundingBox),
	m_confidence(confidence),
	m_data(0),
	m_dataLength(dataLength)
{
	if(!data) return;
	
	m_data = new char[m_dataLength];
	memcpy(m_data, data, m_dataLength);
}

Camera::Object::~Object()
{
	delete[] m_data;
}

const Point2<unsigned> &Camera::Object::centroid() const
{
	return m_centroid;
}

const Rectangle<unsigned> &Camera::Object::boundingBox() const
{
	return m_boundingBox;
}

const double &Camera::Object::confidence() const
{
	return m_confidence;
}

const char *Camera::Object::data() const
{
	return m_data;
}

const size_t &Camera::Object::dataLength() const
{
	return m_dataLength;
}

// Channel //

Camera::Channel::Channel(Device *device, const Config &config)
	: m_device(device)
{
	
}

Camera::Channel::~Channel()
{
	
}

const ObjectVector &Camera::Channel::objects() const
{
	return m_objects;
}

Device *Camera::Channel::device() const
{
	return m_device;
}

void Camera::Channel::setObjects(const ObjectVector &objects)
{
	m_objects = objects;
}

// ConfigPath //

std::string Camera::ConfigPath::s_path = "/etc/botui/channels/";

void Camera::ConfigPath::setBasePath(const std::string &path)
{
	s_path = path;
	if(s_path.empty()) return;
	if(s_path[s_path.size() - 1] != '/') s_path += "/";
}

std::string Camera::ConfigPath::path(const std::string &name)
{
	return s_path + name;
}

// Device //

Camera::Device::Device()
	: m_capture(new cv::VideoCapture)
{
}

Camera::Device::~Device()
{
	delete m_capture;
}

bool Camera::Device::open(const int &number)
{
	if(m_capture->isOpened()) return true;
	return false;
}

void Camera::Device::close()
{
	if(!m_capture->isOpened()) return;
	m_capture->release();
}

void Camera::Device::setConfig(const Config& config)
{
	m_config = config;
	updateConfig();
}

const Config &Camera::Device::config() const
{
	return m_config;
}

void Camera::Device::updateConfig()
{
	m_config.clearGroup();
	m_config.beginGroup(CAMERA_GROUP);
	int numChannels = m_config.intValue(CAMERA_NUM_CHANNELS_KEY);
	if(numChannels <= 0) return;
	for(int i = 0; i < numChannels; ++i) {
		std::stringstream stream;
		stream << CAMERA_CHANNEL_GROUP_PREFIX;
		stream << i;
		m_config.beginGroup(stream.str());
		delete createChannel(m_config.intValue("type"));
		m_config.endGroup();
	}
	m_config.endGroup();
}

Channel *Camera::Device::createChannel(const unsigned &type)
{
	switch(type) {
		case ChannelType::Unknown: return new Private::Camera::UnknownChannel(this, m_config);
		case ChannelType::Color: return new Private::Camera::ColorChannel(this, m_config);;
		case ChannelType::Face: return 0;
		case ChannelType::QR: return 0;
	}
	
	char warn[128];
	sprintf(warn, "Undefined channel type %u", type);
	WARN(warn);
	
	return 0;
}