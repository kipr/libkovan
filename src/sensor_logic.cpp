#include "sensor_logic.hpp"

using namespace SensorLogic;

Base::Base(const Sensor<bool> *a, const Sensor<bool> *b) : m_a(a), m_b(b) {}

const Sensor<bool> *Base::a() const
{
	return m_a;
}

const Sensor<bool> *Base::b() const
{
	return m_b;
}

And::And(const Sensor<bool> *a, const Sensor<bool> *b) : Base(a, b) {}

bool And::value() const
{
	return a()->value() && b()->value();
}

Or::Or(const Sensor<bool> *a, const Sensor<bool> *b) : Base(a, b) {}

bool Or::value() const
{
	return a()->value() || b()->value();
}

Xor::Xor(const Sensor<bool> *a, const Sensor<bool> *b) : Base(a, b) {}

bool Xor::value() const
{
	const bool b = Base::b()->value();
	return a()->value() ? !b : b;
}