#ifndef _NYI_HPP_
#define _NYI_HPP_

#include "kovan/compat.hpp"

#define THIS_IS_NYI nyi(PRETTYFUNC);

#ifdef __cplusplus
extern "C" {
#endif

void nyi(const char *name);

#ifdef __cplusplus
}
#endif

#endif
