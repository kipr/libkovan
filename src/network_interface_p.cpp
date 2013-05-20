#include "network_interface_p.hpp"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>

using namespace Private;

char *NetworkInterface::macAddress(const char *const interface)
{
	ifreq s;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(fd < 0) return 0;
	
	ifaddrs *addrs;
	if(getifaddrs(&addrs)) return 0;
	
	ifaddrs *real = addrs;
	while(real) {
		if(!strcmp(real->ifa_name, interface)) break;
		real = real->ifa_next;
	}
	if(!real) return 0;
	
	static const char *const exampleMacAddress = "01:23:45:67:89:ab";
	const size_t macLength = strlen(exampleMacAddress);
	
	char *const ret = new char[macLength + 1];
	
	static const char hexLookup[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};
	
	char *walker = ret;
	size_t i = 0;
	while(walker <= ret + macLength) {
		const unsigned char &c = real->ifa_addr->sa_data[i++];
		*(++walker) = hexLookup[(c & 0x0F) >> 0];
		*(++walker) = hexLookup[(c & 0xF0) >> 4];
		if(walker <= ret + macLength) *walker = ':';
	}
	ret[macLength] = 0;
	
	freeifaddrs(addrs);
	return ret;
}