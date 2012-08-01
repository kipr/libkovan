#ifndef _SENSOR_LOGIC_HPP_
#define _SENSOR_LOGIC_HPP_

#include "sensor.hpp"

namespace SensorLogic
{
	class Base : public Sensor<bool>
	{
	public:
		Base(const Sensor<bool> *a, const Sensor<bool> *b);
		
		const Sensor<bool> *a() const;
		const Sensor<bool> *b() const;
	private:
		const Sensor<bool> *m_a;
		const Sensor<bool> *m_b;
	};
	
	class And : public Base
	{
	public:
		And(const Sensor<bool> *a, const Sensor<bool> *b);
		virtual bool value() const;
	};
	
	class Or : public Base
	{
	public:
		Or(const Sensor<bool> *a, const Sensor<bool> *b);
		virtual bool value() const;
	};
	
	class Xor : public Base
	{
	public:
		Xor(const Sensor<bool> *a, const Sensor<bool> *b);
		virtual bool value() const;
	};
}

#endif
