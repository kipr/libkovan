#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <netinet/in.h>

typedef int socket_fd_t;

struct sockaddr;

class Address
{
public:
	Address(const char *const host, const unsigned short port);
	Address(const sockaddr_in &addr);
	Address();
	
	bool isValid() const;
	
	bool setHost(const char *const host);
	void setPort(const unsigned short port);
	
	unsigned short port() const;
	
	const sockaddr *addr() const;
	socklen_t addrLength() const;
	
	const char *ip() const;
	
private:
	bool m_valid;
	sockaddr_in m_addr;
};

class Socket
{
public:
	Socket();
	
	bool open(int domain, int type, int protocol);
	bool isOpen() const;
	bool setBlocking(const bool blocking);
	bool setReusable(const bool reusable);
	bool bind(const unsigned short port);
	bool connect(const Address &addr);
	bool disconnect(); 
	
	bool close();
	
	ssize_t recv(void *const buffer, const size_t length, int flags = 0);
	ssize_t recvfrom(void *const buffer, const size_t length, Address &address, int flags = 0);
	
	ssize_t send(const void *const buffer, const size_t length, int flags = 0);
	ssize_t sendto(const void *const buffer, const size_t length, const Address &dest, int flags = 0);
	
	socket_fd_t fd() const;
	
	static Socket udp();
	static Socket tcp();
	
private:
	socket_fd_t m_fd;
};

#endif
