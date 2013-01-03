#include "kovan/camera.hpp"
#include "channel_p.hpp"
#include "warn.hpp"

#include <fstream>
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

Camera::Object::Object(const Object &rhs)
	: m_centroid(rhs.m_centroid),
	m_boundingBox(rhs.m_boundingBox),
	m_confidence(rhs.m_confidence),
	m_data(0),
	m_dataLength(rhs.m_dataLength)
{
	if(!rhs.m_data) return;
	
	m_data = new char[m_dataLength];
	memcpy(m_data, rhs.m_data, m_dataLength);
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

ChannelImpl::ChannelImpl()
	: m_dirty(true)
{
}

ChannelImpl::~ChannelImpl()
{
}

void ChannelImpl::setImage(const cv::Mat &image)
{
	m_image = image;
	m_dirty = true;
}

ObjectVector ChannelImpl::objects(const Config &config)
{
	if(m_dirty) {
		update(m_image);
		m_dirty = false;
	}
	return findObjects(config);
}

ChannelImplManager::~ChannelImplManager()
{
}

DefaultChannelImplManager::DefaultChannelImplManager()
{
	m_channelImpls["hsv"] = new Private::Camera::HsvChannelImpl();
	m_channelImpls["qr"] = new Private::Camera::BarcodeChannelImpl();
}

DefaultChannelImplManager::~DefaultChannelImplManager()
{
	std::map<std::string, ChannelImpl *>::iterator it = m_channelImpls.begin();
	for(; it != m_channelImpls.end(); ++it) delete it->second;
}

void DefaultChannelImplManager::setImage(const cv::Mat &image)
{
	std::map<std::string, ChannelImpl *>::iterator it = m_channelImpls.begin();
	for(; it != m_channelImpls.end(); ++it) it->second->setImage(image);
}

ChannelImpl *DefaultChannelImplManager::channelImpl(const std::string &name)
{
	std::map<std::string, ChannelImpl *>::iterator it = m_channelImpls.find(name);
	return (it == m_channelImpls.end()) ? 0 : it->second;
}

// Channel //

Camera::Channel::Channel(Device *device, const Config &config)
	: m_device(device),
	m_config(config),
	m_impl(0),
	m_valid(false)
{
	m_objects.clear();
	const std::string type = config.stringValue("type");
	if(type.empty()) {
		WARN("No type specified in config.");
		return;
	}
	
	m_impl = device->channelImplManager()->channelImpl(type);
	if(!m_impl) {
		WARN("Type %s not found", type.c_str());
		return;
	}
}

Camera::Channel::~Channel()
{
}

void Camera::Channel::invalidate()
{
	m_valid = false;
}

const ObjectVector *Camera::Channel::objects() const
{
	if(!m_impl) return 0;
	if(!m_valid) {
		m_objects.clear();
		m_objects = m_impl->objects(m_config);
		m_valid = true;
	}
	return &m_objects;
}

Device *Camera::Channel::device() const
{
	return m_device;
}

void Camera::Channel::setConfig(const Config &config)
{
	m_config = config;
	invalidate();
}

// ConfigPath //

std::string Camera::ConfigPath::s_path = "/etc/botui/channels/";

std::string Camera::ConfigPath::extension()
{
	return "conf";
}

void Camera::ConfigPath::setBasePath(const std::string &path)
{
	s_path = path;
	if(s_path.empty()) return;
	if(s_path[s_path.size() - 1] != '/') s_path += "/";
}

std::string Camera::ConfigPath::path(const std::string &name)
{
	if(name.empty()) return s_path;
	return s_path + name + "." + extension();
}

std::string Camera::ConfigPath::defaultPath()
{
	return s_path + "default";
}

std::string Camera::ConfigPath::defaultConfigPath()
{
	std::ifstream file;
	file.open(defaultPath().c_str());
	if(!file.is_open()) return std::string();
	std::string ret;
	std::getline(file, ret);
	file.close();
	return ret;
}

void Camera::ConfigPath::setDefaultConfigPath(const std::string &name)
{
	std::ofstream file;
	file.open(defaultPath().c_str());
	if(!file.is_open()) return;
	const std::string path = ConfigPath::path(name);
	file.write(path.c_str(), path.size());
	file.close();	
}

// Device //

Camera::Device::Device()
	: m_capture(new cv::VideoCapture),
	m_channelImplManager(new DefaultChannelImplManager),
	m_grabCount(1)
{
	Config *config = Config::load(Camera::ConfigPath::defaultConfigPath());
	if(!config) return;
	setConfig(*config);
	delete config;
}

Camera::Device::~Device()
{
	ChannelPtrVector::iterator it = m_channels.begin();
	for(; it != m_channels.end(); ++it) delete *it;
	delete m_capture;
}

bool Camera::Device::open(const int &number)
{
	if(m_capture->isOpened()) return true;
	return m_capture->open(number);
}

bool Camera::Device::isOpen() const
{
	return m_capture->isOpened();
}

void Camera::Device::setWidth(const unsigned &width)
{
	m_capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
}

void Camera::Device::setHeight(const unsigned &height)
{
	m_capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);
}

void Camera::Device::setGrabCount(unsigned char grabs)
{
	if(grabs < 1) grabs = 1;
	else if(grabs > 5) grabs = 5;
	m_grabCount = grabs;
}

unsigned char Camera::Device::grabCount() const
{
	return m_grabCount;
}

void Camera::Device::close()
{
	if(!m_capture->isOpened()) return;
	m_capture->release();
}

bool Camera::Device::update()
{
	// Get new image
	bool success = true;
	for(unsigned char i = 0; i < m_grabCount; ++i) {
		success &= m_capture->grab();
	}
	success &= m_capture->retrieve(m_image);
	
	if(!success) return false;
	
	// No need to update channels if there are none.
	if(m_channels.empty()) return true;
	
	// Dirty all channel impls
	m_channelImplManager->setImage(m_image);
	
	// Invalidate all channels
	ChannelPtrVector::iterator it = m_channels.begin();
	for(; it != m_channels.end(); ++it) (*it)->invalidate();
	return true;
}

const ChannelPtrVector &Camera::Device::channels() const
{
	return m_channels;
}

cv::VideoCapture *Camera::Device::videoCapture() const
{
	return m_capture;
}

const cv::Mat &Camera::Device::rawImage() const
{
	return m_image;
}

void Camera::Device::setConfig(const Config &config)
{
	m_config = config;
	updateConfig();
}

const Config &Camera::Device::config() const
{
	return m_config;
}

void Camera::Device::setChannelImplManager(ChannelImplManager *channelImplManager)
{
	delete m_channelImplManager;
	m_channelImplManager = channelImplManager;
}

ChannelImplManager *Camera::Device::channelImplManager() const
{
	return m_channelImplManager;
}

void Camera::Device::updateConfig()
{
	ChannelPtrVector::iterator it = m_channels.begin();
	for(; it != m_channels.end(); ++it) delete *it;
	m_channels.clear();
	
	m_config.clearGroup();
	m_config.beginGroup(CAMERA_GROUP);
	int numChannels = m_config.intValue(CAMERA_NUM_CHANNELS_KEY);
	if(numChannels <= 0) return;
	for(int i = 0; i < numChannels; ++i) {
		std::stringstream stream;
		stream << CAMERA_CHANNEL_GROUP_PREFIX;
		stream << i;
		m_config.beginGroup(stream.str());
		m_channels.push_back(new Channel(this, m_config));
		m_config.endGroup();
	}
	m_config.endGroup();
}