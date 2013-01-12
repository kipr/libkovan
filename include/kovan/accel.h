#ifndef _ACCEL_H_
#define _ACCEL_H_

#ifdef __cplusplus
extern "C" {
#endif

short accel_x();
short accel_y();
short accel_z();

int accel_calibrate();

#ifdef __cplusplus
}
#endif

#endif
