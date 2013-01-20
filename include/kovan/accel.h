#ifndef _ACCEL_H_
#define _ACCEL_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYM short accel_x();
EXPORT_SYM short accel_y();
EXPORT_SYM short accel_z();

EXPORT_SYM int accel_calibrate();

#ifdef __cplusplus
}
#endif

#endif
