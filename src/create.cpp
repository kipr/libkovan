#include "create.hpp"
#include "create_codes.h"

#include <fcntl.h>
#include <unistd.h>

#include <cstdio>

#define LOW_BYTE(x) ((x) & 0xFF)
#define HIGH_BYTE(x) (((x) & 0xFF00) >> 8)

const static unsigned int baudCodeRate[12] = {
	300, 600, 1200, 2400, 4800, 9600,
	14400, 19200, 28800, 38400, 57600,
	115200
};

CreateScript::CreateScript()
{
	
}

CreateScript::CreateScript(const CreateScript& rhs)
{
	append(rhs);
}

void CreateScript::append(const unsigned char& byte)
{
	append(&byte, 1);
}

void CreateScript::append(const unsigned char *data, const size_t& size)
{
	m_script.reserve(m_script.size() + size);
	for(size_t i = 0; i < size; ++i) m_script.push_back(data[i]);
}

void CreateScript::append(const CreateScript& script)
{
	append(script.data(), script.size());
}

void CreateScript::remove(const size_t& i, const size_t& len)
{
	if(len == 0) return;
	std::vector<unsigned char>::iterator it = m_script.begin();
	m_script.erase(it + i, it + i + len);
}

const unsigned char *CreateScript::data() const
{
	return &m_script[0]; // This is pretty jank
}

size_t CreateScript::size() const
{
	return m_script.size();
}

unsigned char CreateScript::byte(const size_t& i)
{
	return m_script[i];
}

CreateScript& CreateScript::operator=(const CreateScript& rhs)
{
	m_script = rhs.m_script;
	return *this;
}

Create::~Create()
{
	disconnect();
}

int
set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0) {
		perror("tcgetattr");
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // ignore break signal
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
		perror("tcsetattr");
                return -1;
        }
        return 0;
}

bool Create::connect()
{
	if(!open()) return false;
	if(!set_interface_attribs(m_tty, B57600, 0)) return false;
	
	// setLocalBaudRate(baudCodeRate[10]); // This is the default rate
	start();
	
	// Make the connection baud rate 115200
	// setBaudRate(11);
	
	setMode(SafeMode);
	if(mode() != SafeMode) return false;
	
	return true;
	
}

bool Create::disconnect()
{
	close();
	return true;
}

bool Create::isConnected()
{
	return m_tty;
}

void Create::setPassiveMode()
{
	start();
}

void Create::setSafeMode()
{
	write(OI_SAFE);
}

void Create::setFullMode()
{
	write(OI_FULL);
}

void Create::setMode(const Create::Mode& mode)
{
	switch(mode) {
	case OffMode: // NYI
	case PassiveMode: setPassiveMode(); break;
	case SafeMode: setSafeMode(); break;
	case FullMode: setFullMode(); break;
	}
}

Create::Mode Create::mode()
{
	write(OI_SENSORS);
	write(35);
	short state = 0;
	do {
		state = read();
		if(state < 0) return OffMode;
	} while(state == 0);
	printf("State = %d\n", state);
        switch(state) {
	case 0: return OffMode;
	case 1: return PassiveMode;
	case 2: return SafeMode;
	case 3: return FullMode;
	}
	return OffMode;
}

void Create::send(const CreateScript& script)
{
	m_script = script;
}

bool Create::write(const unsigned char& c)
{
	return write(&c, 1);
}

bool Create::write(const unsigned char *data, const size_t& len)
{
	if(!m_tty) return false;
	return ::write(m_tty, data, len) == len;
}

short Create::read()
{
	unsigned char ret = 0;
	;
	return read(&ret, 1) == 1 ? ret : -1;
}

int Create::read(unsigned char *data, const size_t& len)
{
	if(!m_tty) return 0;
	int ret = ::read(m_tty, data, len);
	if(ret < 0) perror("::read");
	return ret;
}

bool Create::setBaudRate(const unsigned char& baudCode)
{
	if(!m_tty || baudCode >= 12) return false;
	
	// Tell the create to update its baud rate
	write(OI_BAUD);
	write(baudCode);
	
	// Update our baud rate
	setLocalBaudRate(baudCodeRate[baudCode]);
	
	return true;
}

Create *Create::instance()
{
	static Create s_create;
	return &s_create;
}

Create::Create() {}
Create::Create(const Create&) {}
Create& Create::operator=(const Create&) { return *this; }

void Create::setLocalBaudRate(const speed_t& baudRate)
{
	struct termios attribs;
	tcgetattr(m_tty, &attribs);
	
	speed_t in = cfgetispeed(&attribs);
	speed_t out = cfgetospeed(&attribs);
	printf("in = %lu, out = %lu (old)\n", in, out);
	
	cfsetispeed(&attribs, baudRate);
	cfsetospeed(&attribs, baudRate);
	printf("baud = %lu (new)\n", baudRate);
	
	tcsetattr(m_tty, TCSADRAIN, &attribs);
}

bool Create::start()
{
	if(!m_tty) return false;
	return write(OI_START);
}

bool Create::open()
{
	if(m_tty) return false;
	m_tty = ::open("/dev/tty.USA19Hfa14P1.1", O_RDWR | O_NONBLOCK);
	if(m_tty < 0) perror("Create::open");
	return m_tty >= 0;
}

void Create::close()
{
	if(!m_tty) return;
	::close(m_tty);
	m_tty = 0;
}