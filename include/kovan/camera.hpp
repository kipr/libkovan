/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
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

#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "geom.hpp"
#include "color.hpp"
#include "config.hpp"
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

// These keys are used in the config files loaded by
// Camera::Device
#define CAMERA_GROUP ("camera")
#define CAMERA_NUM_CHANNELS_KEY ("numChannels")
#define CAMERA_CHANNEL_GROUP_PREFIX ("channel_")

namespace cv
{
	class VideoCapture;
}

namespace Camera
{
	class Device;
	
	class Object
	{
	public:
		Object(const Point2<unsigned> &centroid,
			const Rectangle<unsigned> &boundingBox,
			const double &confidence, const char *data = 0,
			const size_t &dataLength = 0);
		~Object();
		
		const Point2<unsigned> &centroid() const;
		const Rectangle<unsigned> &boundingBox() const;
		const double &confidence() const;
		const char *data() const;
		const size_t &dataLength() const;
		
	private:
		Point2<unsigned> m_centroid;
		Rectangle<unsigned> m_boundingBox;
		double m_confidence;
		char *m_data;
		size_t m_dataLength;
	};
	
	typedef std::vector<Object> ObjectVector;
	
	class Channel
	{
	public:
		Channel(Device *device, const Config &config);
		virtual ~Channel();
		
		const ObjectVector &objects() const;
		
		virtual void update() = 0;
		Device *device() const;
	protected:
		void setObjects(const ObjectVector &objects);
		
	private:
		Device *m_device;
		ObjectVector m_objects;
	};
	
	struct ChannelType
	{
		enum {
			Unknown = 0,
			Color,
			Face,
			QR
		};
	};
	
	class ConfigPath
	{
	public:
		static void setBasePath(const std::string &path);
		static std::string path(const std::string &name);
	private:
		static std::string s_path;
	};
	
	class Device
	{
	public:
		Device();
		~Device();
		
		bool open(const int &number = 0);
		void close();
		
		void setConfig(const Config& config);
		const Config &config() const;
		
	protected:
		virtual Channel *createChannel(const unsigned &type);
		
	private:
		void updateConfig();
		
		Config m_config;
		cv::VideoCapture *m_capture;
	};
}



#endif
