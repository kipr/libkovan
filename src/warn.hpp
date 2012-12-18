#ifndef _WARN_HPP_
#define _WARN_HPP_

#include <cstdio>

#define MAX_MESSAGE_SIZE 256

#define WARN(x, ...) \
	do { \
		fprintf(stderr, "%s: warning: " x "\n", __PRETTY_FUNCTION__, ##__VA_ARGS__); \
	} while(0)

#define PWARN(x, ...) \
	do { \
		char buffer[MAX_MESSAGE_SIZE]; \
		sprintf(buffer, "%s: warning: " x "\n", __PRETTY_FUNCTION__, ##__VA_ARGS__); \
		perror(buffer); \
	} while(0)

#endif
