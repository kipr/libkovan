/**************************************************************************
 *  Copyright 2013 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#ifndef _ARDRONE_HPP_
#define _ARDRONE_HPP_

#include <opencv2/core/core.hpp>
#include "kovan/camera.hpp"

class DroneController;

class EXPORT_SYM ARDrone
{
public:
	enum State
	{
		Disconnected = 0,
		Landed,
		Flying
	};
	
	enum Camera
	{
		None = 0,
		Front,
		Bottom
	};
	
	~ARDrone();
	
	bool connect(const char *const ip = "192.168.1.1", const double timeout = 2.0);
	void disconnect();
	
	void setActiveCamera(const Camera activeCamera);
	Camera activeCamera() const;
	
	void flatTrim();
	void takeoff();
	void land();
	
	void hover();
	void move(const float x, const float y, const float z, const float yaw);
	
	const cv::Mat &rawImage() const;
	
	ARDrone::State state() const;
	
	static ARDrone *instance();
	
private:
	ARDrone();
	
	DroneController *m_controller;
	Camera m_activeCamera;
};

namespace Camera
{
	class EXPORT_SYM ARDroneInputProvider : public InputProvider
	{
	public:
		ARDroneInputProvider();
		virtual bool open(const int number);
		virtual bool isOpen() const;
		virtual void setWidth(const unsigned width);
		virtual void setHeight(const unsigned height);
		virtual bool next(cv::Mat &image);
		virtual bool close();
		
	private:
		bool m_opened;
	};
}

#endif
