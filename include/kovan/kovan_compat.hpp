#ifndef _KOVAN_COMPAT_HPP_
#define _KOVAN_COMPAT_HPP_

#ifdef _MSC_VER

#include <basetsd.h>
typedef SSIZE_T ssize_t;
#define PRETTYFUNC __FUNCSIG__
#define EXITFUNC(x) _exit(x)
#pragma section(".CRT$XCU", read)
#define INITIALIZER(f) \
   static void __cdecl f(void); \
   __declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f; \
   static void __cdecl f(void)

#else

#define PRETTYFUNC __PRETTY_FUNCTION__
#define EXITFUNC(x) _Exit(x)
#define INITIALIZER(f) \
	static void f(void) __attribute__((constructor)); \
	static void f(void)

#endif

#endif