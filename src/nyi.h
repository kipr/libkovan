#ifndef _NYI_HPP_
#define _NYI_HPP_

#define THIS_IS_NYI nyi(__func__);

#ifdef __cplusplus
extern "C" {
#endif

void nyi(const char *name);

#ifdef __cplusplus
}
#endif

#endif
