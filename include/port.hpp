#ifndef _PORT_HPP_
#define _PORT_HPP_

#include <stdexcept>

class InvalidPort : std::logic_error
{
public:
	InvalidPort(const std::string& what);
};

typedef int port_t;

#endif
