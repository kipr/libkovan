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

class DroneController;

class ARDrone
{
public:
	enum State
	{
		Disconnected = 0,
		Landed,
		Flying
	};
	
	~ARDrone();
	
	bool connect(const char *const ip = "192.168.1.1");
	void disconnect();
	
	void flatTrim();
	void takeoff();
	void land();
	
	void hover();
	void move(const float x, const float y, const float z, const float yaw);
	
	cv::Mat video() const;
	
	ARDrone::State state() const;
	
	static ARDrone *instance();
	
private:
	ARDrone();
	
	DroneController *m_controller;
};



#endif
