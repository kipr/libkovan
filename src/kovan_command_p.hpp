#ifndef _KOVAN_COMMAND_P_HPP_
#define _KOVAN_COMMAND_P_HPP_

#define MAX_COMMAND_DATA_SIZE 16
#define NUM_FPGA_REGS 48
#define TOTAL_REGS 200

namespace Private
{
	enum CommandType
	{
		NilType = 0,
		StateCommandType,
		WriteCommandType
	};

	struct Command
	{
		enum CommandType type;
		unsigned char data[MAX_COMMAND_DATA_SIZE];
	};

	struct Packet
	{
		unsigned short num;
		struct Command commands[1];
	};

	struct WriteCommand
	{
		unsigned short addy; // 0 - 40
		unsigned short val; // 0 - 0xFFFF
	};

	struct State
	{
		unsigned short t[TOTAL_REGS];
	};
	
	Command createWriteCommand(const unsigned short &address, const unsigned short &value);
}

#endif
