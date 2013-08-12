#ifndef _KOVAN_COMPAT_HPP_
#define _KOVAN_COMPAT_HPP_

#ifdef _MSC_VER
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#define PRETTYFUNC __FUNCSIG__
#define EXITFUNC(x) _exit(x)
#else
#define PRETTYFUNC __PRETTY_FUNCTION__
#define EXITFUNC(x) _Exit(x)
#endif

#endif