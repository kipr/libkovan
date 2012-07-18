#ifndef _SERVOS_H_
#define _SERVOS_H_

#ifdef __cplusplus
extern "C" {
#endif

int get_servo_position();
void set_servo_position(int position);

#ifdef __cplusplus
}
#endif


#endif
