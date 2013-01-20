#ifndef _PORT_HPP_
#define _PORT_HPP_

#include "export.h"

#include <stdexcept>

class EXPORT_SYM InvalidPort : std::logic_error
{
public:
	InvalidPort(const std::string& what);
};

typedef int port_t;

#endif
