#include "kovan_module_p.hpp"

VH #include "kovan/compat.hpp"
#include "kovan_regs_p.hpp"

#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <errno.h>

#ifndef WIN32
#include <unistd.h>
#endif

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
	bool on = true;
	setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&on), sizeof(bool));
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

	while(::bind(m_sock, (sockaddr *)&sa, sizeof(sa)) < 0) {
		// Keep trying to bind until we find an open port
#ifndef WIN32
		if(errno == EADDRINUSE) {
#else
		if(errno == WSAEADDRINUSE) {
#endif
			++sa.sin_port;
			continue;
		}
		
		perror("bind");
		return false;
	}
	
	return true;
}

void KovanModule::close()
{
	if(m_sock < 0) return;
#ifndef WIN32
	::close(m_sock);
#else
	closesocket(m_sock);
#endif
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
	if(packet == NULL) { //As createPacket now can return NULL, we have to check for this
	    return false;
	}
  
	memcpy(packet->commands, &commands[0], commands.size() * sizeof(Command));
	bool ret = true;
	while(sendto(m_sock, reinterpret_cast<const char *>(packet), packetSize, 0,
		(sockaddr *)&m_out, sizeof(m_out)) != packetSize) {
		if(errno == EINTR) continue;
#ifdef WIN32
		std::cout << "Windows error code: " << WSAGetLastError() << std::endl;
#endif
		perror("sendto");
		ret = false;
	}
	
	free(packet);
	return ret;
}

bool KovanModule::recv(State& state)
{
	memset(&state, 0, sizeof(State));
	
	ssize_t i = 0;
	while((i = recvfrom(m_sock, reinterpret_cast<char *>(&state),
		sizeof(State), 0, NULL, NULL)) != sizeof(State)) {
		if(errno == EINTR) continue;
#ifdef WIN32
		std::cout << "Windows error code: " << WSAGetLastError() << std::endl;
#endif
		perror("recvfrom");
		printf("Got %ld\n", i);
		return false;
	}
	
	return true;
}

Packet *KovanModule::createPacket(const uint16_t& num, uint32_t& packet_size)
{
	packet_size = sizeof(Packet) + sizeof(Command) * (num - 1);
	void *buffer = malloc(packet_size);
	if(buffer == NULL) { //malloc returns NULL if allocating the memory fails
	    return NULL;
	}
	Packet *packet = reinterpret_cast<Packet *>(buffer);
	packet->num = num; //<-- this crashes if malloc returns 0, as -> tries to dereference it
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

#ifdef WIN32
#include <winsock.h>
#include <windows.h>
#include <stdio.h>

#include "compat.hpp"

// Derived from: https://github.com/jamescoxon/dl-fldigi/blob/master/src/compat/mingw.c
INITIALIZER(wsa_init)
{
	WSADATA wsa;
	static int s_init = 0;
	if (s_init) return;

	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
		fprintf(stderr, "ERROR: WSAStartup failed. Code: %d\n", WSAGetLastError());
		fprintf(stderr, "ERROR: Network communication will not work.\n");
	}

	atexit((void(*)(void)) WSACleanup);
	s_init = 1;
}
#endif