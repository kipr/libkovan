#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

void debug_print_registers();

unsigned short register_value(unsigned short addy);

#ifdef __cplusplus
}
#endif

#endif
