#ifndef _SENSORS_HPP_
#define _SENSORS_HPP_

template<typename T>
class Sensor
{
public:
	virtual T value() const = 0;
};

#endif
