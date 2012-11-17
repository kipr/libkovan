#include "kovan_command_p.hpp"

#include <cstring>

using namespace Private;

Command Private::createWriteCommand(const unsigned short &address, const unsigned short &value)
{
	WriteCommand wc;
	wc.addy = address;
	wc.val = value;
	Command c0;
	c0.type = WriteCommandType;
	memcpy(c0.data, &wc, sizeof(WriteCommand));
	return c0;
}