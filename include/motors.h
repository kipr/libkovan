#ifndef _MOTORS_H_
#define _MOTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

int get_motor_position_counter(int motor);
int clear_motor_position_counter(int motor);
int move_at_velocity(int motor, int velocity);
int mav(int motor, int velocity);
int move_to_position(int motor, int speed, int goal_pos);
int mtp(int motor, int speed, int goal_pos);
int move_relative_position(int motor, int speed, int delta_pos);
int mrp(int motor, int speed, int delta_pos);
void set_pid_gains(int motor, int p, int i, int d, int pd, int id, int dd);
void get_pid_gains(int motor, int *p, int *i, int *d, int *pd, int *id, int *dd);
int freeze(int motor);
int get_motor_done(int motor);
void block_motor_done(int motor);
void bmd(int motor);
int setpwm(int motor, int pwm);
int getpwm(int motor);
void fd(int motor);
void bk(int motor);
void motor (int motor, int percent);
void off(int motor);
void ao();

#ifdef __cplusplus
}
#endif


#endif
