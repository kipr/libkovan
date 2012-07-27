#ifndef _CREATE_HPP_
#define _CREATE_HPP_

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <termios.h>
#include <sys/time.h>

class CreateScript
{
public:
	CreateScript();
	CreateScript(const CreateScript& rhs);
	
	void append(const unsigned char& byte);
	void append(const unsigned char *data, const size_t& size);
	void append(const CreateScript& script);
	
	void remove(const size_t& i, const size_t& len = 1);
	
	const unsigned char *data() const;
	size_t size() const;
	
	unsigned char byte(const size_t& i);
	
	CreateScript& operator=(const CreateScript& rhs);
	
private:
	std::vector<unsigned char> m_script;
};

class Create
{
public:
	enum Mode {
		OffMode,
		PassiveMode,
		SafeMode,
		FullMode
	};
	
	~Create();
	
	bool connect();
	bool disconnect();
	bool isConnected();
	
	void setPassiveMode();
	void setSafeMode();
	void setFullMode();
	
	void setMode(const Create::Mode& mode);
	Create::Mode mode();
	
	void send(const CreateScript& script);
	
	bool write(const unsigned char& c);
	bool write(const unsigned char *data, const size_t& len);
	
	short read();
	int read(unsigned char *data, const size_t& len);
	
	bool setBaudRate(const unsigned char& baudCode);
	
	static Create *instance();
	
private:
	Create();
	Create(const Create& rhs);
	Create& operator=(const Create& rhs);
	
	void setLocalBaudRate(const speed_t& baudRate);
	
	bool start();
	
	bool open();
	void close();
	
	size_t m_i;
	CreateScript m_script;
	int m_tty;
	
};

#endif
