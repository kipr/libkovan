#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "export.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYM void debug_print_registers();

EXPORT_SYM unsigned short register_value(unsigned short addy);

EXPORT_SYM int debug_dump_data(const unsigned char *const data, const size_t size, const char *const where);

#ifdef __cplusplus
}
#endif

#endif
