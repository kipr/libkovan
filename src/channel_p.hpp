#ifndef _CHANNEL_P_HPP_
#define _CHANNEL_P_HPP_

#include "kovan/camera.hpp"
#include <opencv2/core/core.hpp>
#include <zbar.h>
#include <map>

namespace Private
{
	namespace Camera
	{
		class HsvChannelImpl : public ::Camera::ChannelImpl
		{
		public:
			HsvChannelImpl();
			virtual void update(const cv::Mat &image);
			virtual ::Camera::ObjectVector findObjects(const Config &config);
			
		private:
			cv::Mat m_image;
		};
		
		class BarcodeChannelImpl : public ::Camera::ChannelImpl
		{
		public:
			BarcodeChannelImpl();
			virtual void update(const cv::Mat &image);
			virtual ::Camera::ObjectVector findObjects(const Config &config);

		private:
			cv::Mat m_gray;
			zbar::Image m_image;
			zbar::ImageScanner m_scanner;
		};
	}
}

#endif
