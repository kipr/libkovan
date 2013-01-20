#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYM void debug_print_registers();

EXPORT_SYM unsigned short register_value(unsigned short addy);

#ifdef __cplusplus
}
#endif

#endif
