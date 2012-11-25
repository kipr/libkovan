#include "kovan_module_p.hpp"

#include "kovan_regs_p.hpp"

#include <iostream>

#include <cstdio>
#include <cstdlib>

#define TIMEDIV (1.0 / 13000000) // 13 MHz clock
#define PWM_PERIOD_RAW 0.02F
#define SERVO_MAX_RAW 0.002f
#define SERVO_MIN_RAW 0.001f
#define PWM_PERIOD ((unsigned int)(PWM_PERIOD_RAW / TIMEDIV))
#define SERVO_MAX (SERVO_MAX_RAW / TIMEDIV)
#define SERVO_MIN (SERVO_MIN_RAW / TIMEDIV)

using namespace Private;

KovanModule::KovanModule(const uint64_t& moduleAddress, const uint16_t& modulePort)
	: m_sock(-1)
{
	memset(&m_out, 0, sizeof(m_out));
	m_out.sin_family = AF_INET;
	m_out.sin_addr.s_addr = moduleAddress;
	m_out.sin_port = modulePort;
}

KovanModule::~KovanModule()
{
	close();
}

bool KovanModule::init()
{
	// Socket was already inited
	if(m_sock >= 0) return true;
	
	m_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_sock < 0) {
		perror("socket");
		return false;
	}
	
	return true;
}

bool KovanModule::bind(const uint64_t& address, const uint16_t& port)
{
	if(m_sock < 0) return false;
	
	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET; 
	sa.sin_addr.s_addr = address;
	sa.sin_port = port;

	if(::bind(m_sock, (sockaddr *)&sa, sizeof(sa)) < 0) {
		perror("bind");
		return false;
	}
	
	return true;
}

void KovanModule::close()
{
	if(m_sock < 0) return;
	
	::close(m_sock);
}

uint64_t KovanModule::moduleAddress() const
{
	return m_out.sin_addr.s_addr;
}

uint16_t KovanModule::modulePort() const
{
	return m_out.sin_port;
}

bool KovanModule::send(const Command& command)
{
	return send(CommandVector(1, command));
}

bool KovanModule::send(const CommandVector& commands)
{
	uint32_t packetSize = 0;
	Packet *packet = createPacket(commands.size(), packetSize);
	memcpy(packet->commands, &commands[0], commands.size() * sizeof(Command));
	
	bool ret = true;
	if(sendto(m_sock, packet, packetSize, 0, (sockaddr *)&m_out, sizeof(m_out)) != packetSize) {
		perror("sendto");
		ret = false;
	}
	
	free(packet);
	return ret;
}

bool KovanModule::recv(State& state)
{
	memset(&state, 0, sizeof(State));
	if(recvfrom(m_sock, &state, sizeof(State), 0, NULL, NULL) != sizeof(State)) {
		perror("recvfrom");
		return false;
	}
	return true;
}

Packet *KovanModule::createPacket(const uint16_t& num, uint32_t& packet_size)
{
	packet_size = sizeof(Packet) + sizeof(Command) * (num - 1);
	Packet *packet = reinterpret_cast<Packet *>(malloc(packet_size));
	packet->num = num;
	return packet;
}

int KovanModule::getState(State &state)
{
	Command c0;
	c0.type = StateCommandType;

	CommandVector commands;
	commands.push_back(c0);

	send(commands);

	if(!recv(state)) {
		std::cout << "Error: didn't get state back!" << std::endl;
		return -1;
	}

	return 0;
}

void KovanModule::displayState(const State &state)
{
	int i;
	std::cout << "State: " << std::endl;
	for (i = 0; i < NUM_FPGA_REGS; i+=4){
		std::cout << "\t"
				<< state.t[i]   << ", "
				<< state.t[i+1] << ", "
				<< state.t[i+2] << ", "
				<< state.t[i+3] << std::endl;
	}
}
