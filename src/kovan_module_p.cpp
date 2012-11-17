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

// test to turn all 4 motors on
//
// pwm_div is probably constant
//
// pwm_val depends on the speed  (shared for all motors currently)
//
// set speedPercent 0 - 100
//
// drive_code specifies  forward/reverse/idle/brake
// Forward = 10, Reverse = 01, Brake = 11, Idle = 00
void KovanModule::turnMotorsOn(const unsigned short &speedPercent)
{
	unsigned short speed = 0;

	if(speedPercent > 100) {
		speed = 2600;
	} else {
		speed = (speedPercent * 2600) / 100;
	}


	Command c0 = createWriteCommand(MOTOR_PWM_0, speed);
	Command c1 = createWriteCommand(MOTOR_PWM_1, speed);
	Command c2 = createWriteCommand(MOTOR_PWM_2, speed);
	Command c3 = createWriteCommand(MOTOR_PWM_3, speed);

	// drive code (all forward)
	Command c4 = createWriteCommand(MOTOR_DRIVE_CODE_T, 0xAA);

	CommandVector commands;
	commands.push_back(c0);
	commands.push_back(c1);
	commands.push_back(c2);
	commands.push_back(c3);
	commands.push_back(c4);

	// annoying  that we have to do this
	Command r0;
	r0.type = StateCommandType;
	commands.push_back(r0);

	send(commands);
	State state;

	// shouldn't need this
	if(!recv(state)) {
		std::cout << "Error: didn't get state back!" << std::endl;
		return;
	}
}

void KovanModule::turnMotorsOff()
{
	// speed 0
	Command c0 = createWriteCommand(MOTOR_PWM_0, 0);
	Command c1 = createWriteCommand(MOTOR_PWM_1, 0);
	Command c2 = createWriteCommand(MOTOR_PWM_2, 0);
	Command c3 = createWriteCommand(MOTOR_PWM_3, 0);

	// passive brake
	Command c4 = createWriteCommand(MOTOR_DRIVE_CODE_T, 0);

	CommandVector commands;
	commands.push_back(c0);
	commands.push_back(c1);
	commands.push_back(c2);
	commands.push_back(c3);
	commands.push_back(c4);

	// annoying  that we have to do this
	Command r0;
	r0.type = StateCommandType;
	commands.push_back(r0);

	send(commands);
	State state;

	// shouldn't need this
	if(!recv(state)) {
		std::cout << "Error: didn't get state back!" << std::endl;
		return;
	}
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


unsigned char KovanModule::readDigitals()
{
	std::cout << "Warning: readDigitals has not been tested." << std::endl; // TODO

	State state;
	getState(state);

	unsigned char pinVals = 0;
	pinVals = (state.t[DIG_OUT] & state.t[DIG_OUT_ENABLE]);
	pinVals |= (state.t[DIG_IN] & ~state.t[DIG_OUT_ENABLE]);

	return pinVals;
}


int KovanModule::singleWrite(const unsigned short &address, const unsigned short &value)
{
	Command c0 = createWriteCommand(address, value);

	CommandVector commands;
	commands.push_back(c0);

	// annoying  that we have to do this
	Command r0;
	r0.type = StateCommandType;
	commands.push_back(r0);

	State state;

	// shouldn't need this
	if(!recv(state)) {
		std::cout << "Error: didn't get state back!" << std::endl;
		return -1;
	}

	return 0;
}


// this version just replaces everything
void KovanModule::writeDigitals(const unsigned char &values, const unsigned char &pullups, const unsigned char &outputEnables)
{
	std::cout << "Warning: writeDigitals has not been tested." << std::endl; // TODO

	Command c0 = createWriteCommand(DIG_OUT, values);
	Command c1 = createWriteCommand(DIG_OUT_ENABLE, outputEnables);
	Command c2 = createWriteCommand(DIG_PULLUPS, pullups);

	CommandVector commands;
	commands.push_back(c0);
	commands.push_back(c1);
	commands.push_back(c2);

	// annoying  that we have to do this
	Command r0;
	r0.type = StateCommandType;
	commands.push_back(r0);

	send(commands);
	State state;

	// shouldn't need this
	if(!recv(state)) {
		std::cout << "Error: didn't get state back!" << std::endl;
		return;
	}

	singleWrite(DIG_UPDATE_T, 1);
	singleWrite(DIG_UPDATE_T, 0);
	singleWrite(DIG_SAMPLE_T, 0); // FPGA captures new dig values
	singleWrite(DIG_SAMPLE_T, 1);
}


// channel 0 - 15
unsigned short KovanModule::getADC(const unsigned short &channel)
{
	unsigned short adc_val = 0xFFFF;

	// Write adc_chan without go_bit
	Command w0 = createWriteCommand(ADC_GO_T, 0);
	Command w1 = createWriteCommand(ADC_CHAN_T, channel);

	// write go bit high
	Command w2 = createWriteCommand(ADC_GO_T, 1);

	// write go bit low
	Command w3 = createWriteCommand(ADC_GO_T, 0);

	CommandVector writeCommands;
	writeCommands.push_back(w0);
	writeCommands.push_back(w1);
	writeCommands.push_back(w2);
	writeCommands.push_back(w3);

	// annoying that we have to do this
	Command r0;
	r0.type = StateCommandType;
	writeCommands.push_back(r0);

	for(int i = 0; i < 2; i++){
		State state;

		send(writeCommands);

		// shouldn't need this
		if(!recv(state)) {
			std::cout << "Error: didn't get state back!" << std::endl;
			return -1;
		}

		// Wait for ready
		do{
			getState(state);
		}while(!state.t[ADC_VALID_T]);

		// Read raw voltage
		adc_val = state.t[ADC_IN_T];
	}

	return adc_val;
}



void KovanModule::setADCPullups(const unsigned char &pullups)
{
	std::cout << "Warning: setADCPullups has not been tested." << std::endl; // TODO
	singleWrite(AN_PULLUPS, pullups);
}

void KovanModule::moveServo(const char &servoNum, const unsigned short &position)
{
	unsigned short val = (unsigned int)(((SERVO_MAX - SERVO_MIN) * (position / 1024.0)) + SERVO_MIN) >> 8;

	// we are only writing to the upper 16 bits of the 24 bit register .....

	unsigned short addy = 0;

	switch(servoNum){
	case 0:
		addy = SERVO_COMMAND_0;
		break;
	case 1:
		addy = SERVO_COMMAND_1;
		break;
	case 2:
		addy = SERVO_COMMAND_2;
		break;
	case 3:
		addy = SERVO_COMMAND_3;
		break;
	default:
		std::cout << "Invalid servo number" << std::endl;
		return;
	}


	Command c0;
	c0.type = StateCommandType;

	// servo position (pwm)
	Command c1 = createWriteCommand(addy, val);

	// Servo PWM period only has to be set once
	// value is computed as  0x03F7A0
	// but we only have access to upper 16 bits
	// Command c2 = createWriteCommand(SERVO_PWM_PERIOD_T, 0x03F7);

	CommandVector commands;
	commands.push_back(c0);
	commands.push_back(c1);

	State state;

	send(commands);
	if(!recv(state)) {
		std::cout << "Error: didn't get state back!" << std::endl;
		return;
	}
}


void KovanModule::speedTest()
{
	Command c0;
	c0.type = StateCommandType;

	Command c1 = createWriteCommand(MOTOR_PWM_0, 3);

	CommandVector commands;
	commands.push_back(c0);
	commands.push_back(c1);

	State state;
	for (int i = 0; i < 1000; i++) {
		send(commands);
		if(!recv(state)) {
			std::cout << "Error: didn't get state back!" << std::endl;
			return;
		}
	}
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
