#ifndef _WARN_HPP_
#define _WARN_HPP_

#include <cstdio>

#define MAX_MESSAGE_SIZE 256

#define WARN(x) \
	do { \
		fprintf(stderr, "%s: warning: %s\n", __PRETTY_FUNCTION__, (x)); \
	} while(0)

#define PWARN(x) \
	do { \
		char buffer[MAX_MESSAGE_SIZE]; \
		sprintf(buffer, "%s: warning: %s\n", __PRETTY_FUNCTION__, (x)); \
		perror(buffer); \
	} while(0)

#endif
