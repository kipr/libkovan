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
#include <map>
#include <string>

#include "kovan/camera.hpp"
#include "kovan/types.hpp"

class DroneController;

namespace Private
{
	class ARDroneEmergencyStop;
}

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
	
	enum Version
	{
		Unknown = 0,
		V1,
		V2
	};
	
	struct NavigationData
	{
		uint32_t battery;
		
		float pitch;
		float roll;
		float yaw;
		
		float altitude;
		
		Vec3f velocity;
		Vec3f position;
	};
	
	~ARDrone();
	
	
	bool connect(const char *const ip = "192.168.1.1", const double timeout = 3.0);
	
	/**
	 * Disconnects from the AR.Drone. The AR.Drone will perform an emergency landing (violently falling).
	 */
	void disconnect();
	
	/**
	 * Gets the version of the connected AR.Drone
	 */
	Version version() const;
	
	NavigationData navigationData() const;
	
	void clearPosition();
	
	/**
	 * Sets the SSID of the drone. The drone must be rebooted for this
	 * change to take effect.
	 * \param ssid The new ssid
	 */
	void setSsid(const char *const ssid);
	
	void setActiveCamera(const Camera activeCamera);
	Camera activeCamera() const;
	
	void flatTrim();
	void takeoff();
	void land();
	
	void setEmergencyStopEnabled(const bool emergencyStopEnabled);
	bool isEmergencyStopEnabled() const;
	
	void setOwnerAddress(const char *const address);
	void pair();
	
	/**
	 * Tells the AR.Drone to hover. This is equivalent to move(0.0f, 0.0f, 0.0f, 0.0f)
	 * \see move
	 */
	void hover();
	
	void move(const float x, const float y, const float z, const float yaw);
	
	std::map<std::string, std::string> configuration() const;
	
	void rawImage(cv::Mat &image) const;
	
	ARDrone::State state() const;
	
	static ARDrone *instance();
	
private:
	ARDrone();
	ARDrone(const ARDrone &);
	
	ARDrone &operator =(const ARDrone &rhs);
	
	DroneController *m_controller;
	Private::ARDroneEmergencyStop *m_emergencyStop;
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
