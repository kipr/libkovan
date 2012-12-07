#ifndef _CHANNEL_COLOR_P_HPP_
#define _CHANNEL_COLOR_P_HPP_

#include "kovan/camera.hpp"
#include <opencv2/core/core.hpp>

namespace Private
{
	namespace Camera
	{
		class UnknownChannel : public ::Camera::Channel
		{
		public:
			UnknownChannel(::Camera::Device *device, const Config &config);
			virtual void update();
		};
		
		class ColorChannel : public ::Camera::Channel
		{
		public:
			ColorChannel(::Camera::Device *device, const Config &config);
			virtual void update();
			
		private:
			cv::Scalar m_bottom;
			cv::Scalar m_top;
		};
	}
}

#endif
