/*!
 * \file create.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup create iRobot (R) Create (TM)
 */


#ifndef _CREATE_H_
#define _CREATE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Attempts to establish a connection to the create.
 * \return 1 if connection succeeded, 0 if connection failed
 * \see create_disconnect
 * \ingroup create
 */
int create_connect();

/*!
 * Cleans up connection to the create.
 * \see create_connect
 * \ingroup create
 */
void create_disconnect() ;
void create_start();
void create_passive();
void create_safe();
void create_full() ;
void create_spot() ;
void create_cover();
void create_demo(int d) ;
void create_cover_dock();
int get_create_mode();
int get_create_lbump();
int get_create_rbump();
int get_create_lwdrop();
int get_create_cwdrop();
int get_create_rwdrop();
int get_create_wall();
int get_create_lcliff();
int get_create_lfcliff();
int get_create_rfcliff();
int get_create_rcliff();
int get_create_vwall();
int get_create_overcurrents();
int get_create_infrared();
int get_create_advance_button();
int get_create_play_button();
int get_create_normalized_angle();
void set_create_normalized_angle(int angle);
int get_create_total_angle();
void set_create_total_angle(int angle);
int get_create_distance();
void set_create_distance(int dist);
int get_create_battery_charging_state();
int get_create_battery_voltage();
int get_create_battery_current();
int get_create_battery_temp();
int get_create_battery_charge();
int get_create_battery_capacity();
int get_create_wall_amt();
int get_create_lcliff_amt();
int get_create_lfcliff_amt();
int get_create_rfcliff_amt();
int get_create_rcliff_amt();
int get_create_bay_DI();
int get_create_bay_AI();
int get_create_song_number();
int get_create_song_playing();
int get_create_number_of_stream_packets();
int get_create_requested_velocity();
int get_create_requested_radius();
int get_create_requested_right_velocity();
int get_create_requested_left_velocity();
void create_stop();
void create_drive (int speed, int radius);
void create_drive_straight (int speed);
void create_spin_CW (int speed);
void create_spin_CCW (int speed);
void create_drive_direct(int r_speed, int l_speed);
int create_spin_block(int speed, int angle);
int _create_get_raw_encoders(long *lenc, long *renc);
void create_advance_led(int on) ;
void create_play_led(int on) ;
void create_power_led(int color, int brightness) ;
void create_digital_output(int bits);
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0);
void create_low_side_drivers(int pwm2, int pwm1, int pwm0);
void create_load_song(int num);
void create_play_song(int num);
int create_read_block(char *data, int count);
void create_write_byte(char byte);
void create_clear_serial_buffer();

#ifdef __cplusplus
}
#endif

#endif
