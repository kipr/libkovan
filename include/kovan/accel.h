#ifndef _ACCEL_H_
#define _ACCEL_H_

#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C" {
#endif

VF EXPORT_SYM short accel_x();

VF EXPORT_SYM short accel_y();

VF EXPORT_SYM short accel_z();

VF EXPORT_SYM int accel_calibrate();

VFL

#ifdef __cplusplus
}
#endif

#endif
