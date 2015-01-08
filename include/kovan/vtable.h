#ifndef __KOVAN_VTABLE_H__
#define __KOVAN_VTABLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "export.h"

#define VF
#define VFL
#define VI
#define VIL
#define VH

#define VTABLE_FUNC_VOID(name, signature, args)                       \
  typedef void (*name##_func) signature;                              \
  EXPORT_SYM extern name##_func g_##name##_func;                                 \
  EXPORT_SYM extern const name##_func g_##name##_func_default;                   \
  static const char *const name##_signature = "void" #signature;      \
  static inline void name signature { (*g_##name##_func) args;  }

#define VTABLE_FUNC(name, returnType, signature, args)                       \
  typedef returnType (*name##_func) signature;                               \
  EXPORT_SYM extern name##_func g_##name##_func;                                        \
  EXPORT_SYM extern const name##_func g_##name##_func_default;                          \
  static const char *const name##_signature = #returnType #signature;        \
  static inline returnType name signature { return (*g_##name##_func) args;  }

#define VTABLE_SET_DEFAULT(name, impl)                  \
  const name##_func g_##name##_func_default = &impl;    \
  name##_func g_##name##_func = g_##name##_func_default

#ifdef __cplusplus
}
#endif

#endif