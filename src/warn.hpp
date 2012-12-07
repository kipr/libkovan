#ifndef _WARN_HPP_
#define _WARN_HPP_

#include <cstdio>

#define WARN(x) \
	do { \
		fprintf(stderr, "%s: warning: %s\n", __PRETTY_FUNCTION__, (x)); \
	} while(0)

#endif
