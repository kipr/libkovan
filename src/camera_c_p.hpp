#ifndef _CAMERA_C_P_HPP_
#define _CAMERA_C_P_HPP_

#include "kovan/camera.hpp"

namespace Private
{
	class DeviceSingleton
	{
	public:
		static void setInputProvider(Camera::InputProvider *const inputProvider);
		
		static Camera::Device *instance();
		
	private:
		static Camera::Device *s_device;
		static Camera::InputProvider *s_inputProvider;
	};
}

#endif
