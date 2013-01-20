#ifndef __EXPORT_DLL_H__
#define __EXPORT_DLL_H__

#ifdef WIN32
#define EXPORT_SYM __declspec(dllexport)
#else
#define EXPORT_SYM
#endif

#endif