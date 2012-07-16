#include "port.hpp"

InvalidPort::InvalidPort(const std::string& what)
	: std::logic_error(what)
{
	
}