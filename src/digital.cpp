#include "digital.hpp"
#include "digital_p.hpp"

Digital::Digital(const unsigned char& port);

bool Digital::setValue(const bool& value);

/*!
 * Gets the current value of the digital sensor
 */
bool Digital::value() const
{
	
}

void Digital::setOutput(const bool& output)
{
	Private::Digital
}

bool Digital::isOutput() const
{
	
}

void Digital::setPullup(const bool& pullup)
{
	
}

bool Digital::pullup() const
{
	
}