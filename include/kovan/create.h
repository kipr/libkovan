/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

/*!
 * \file create.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup create iRobot (R) Create (TM)
 */


#ifndef _CREATE_H_
#define _CREATE_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Waits to establish a connection to the create.
 * \blocksuntil A connection to a create is established
 * \return 0 on success
 * \see create_disconnect
 * \ingroup create
 */
EXPORT_SYM int create_connect();

/*!
 * Attempts to establish a connection to the create.
 * \return 1 if connection succeeded, 0 if connection failed
 * \see create_disconnect
 * \ingroup create
 */
EXPORT_SYM int create_connect_once();

/*!
 * Cleans up connection to the create.
 * \see create_connect
 * \ingroup create
 */
EXPORT_SYM void create_disconnect() ;
EXPORT_SYM void create_start();
EXPORT_SYM void create_passive();
EXPORT_SYM void create_safe();
EXPORT_SYM void create_full() ;
EXPORT_SYM void create_spot() ;
EXPORT_SYM void create_cover();
EXPORT_SYM void create_demo(int d) ;
EXPORT_SYM void create_cover_dock();
EXPORT_SYM int get_create_mode();
EXPORT_SYM int get_create_lbump();
EXPORT_SYM int get_create_rbump();
EXPORT_SYM int get_create_lwdrop();
EXPORT_SYM int get_create_cwdrop();
EXPORT_SYM int get_create_rwdrop();
EXPORT_SYM int get_create_wall();
EXPORT_SYM int get_create_lcliff();
EXPORT_SYM int get_create_lfcliff();
EXPORT_SYM int get_create_rfcliff();
EXPORT_SYM int get_create_rcliff();
EXPORT_SYM int get_create_vwall();
EXPORT_SYM int get_create_overcurrents();
EXPORT_SYM int get_create_infrared();
EXPORT_SYM int get_create_advance_button();
EXPORT_SYM int get_create_play_button();
EXPORT_SYM int get_create_normalized_angle();
EXPORT_SYM void set_create_normalized_angle(int angle);
EXPORT_SYM int get_create_total_angle();
EXPORT_SYM void set_create_total_angle(int angle);
EXPORT_SYM int get_create_distance();
EXPORT_SYM void set_create_distance(int dist);
EXPORT_SYM int get_create_battery_charging_state();
EXPORT_SYM int get_create_battery_voltage();
EXPORT_SYM int get_create_battery_current();
EXPORT_SYM int get_create_battery_temp();
EXPORT_SYM int get_create_battery_charge();
EXPORT_SYM int get_create_battery_capacity();
EXPORT_SYM int get_create_wall_amt();
EXPORT_SYM int get_create_lcliff_amt();
EXPORT_SYM int get_create_lfcliff_amt();
EXPORT_SYM int get_create_rfcliff_amt();
EXPORT_SYM int get_create_rcliff_amt();
EXPORT_SYM int get_create_bay_DI();
EXPORT_SYM int get_create_bay_AI();
EXPORT_SYM int get_create_song_number();
EXPORT_SYM int get_create_song_playing();
EXPORT_SYM int get_create_number_of_stream_packets();
EXPORT_SYM int get_create_requested_velocity();
EXPORT_SYM int get_create_requested_radius();
EXPORT_SYM int get_create_requested_right_velocity();
EXPORT_SYM int get_create_requested_left_velocity();
EXPORT_SYM void create_stop();
EXPORT_SYM void create_drive (int speed, int radius);
EXPORT_SYM void create_drive_straight (int speed);
EXPORT_SYM void create_spin_CW (int speed);
EXPORT_SYM void create_spin_CCW (int speed);
EXPORT_SYM void create_drive_direct(int l_speed, int r_speed);
EXPORT_SYM int create_spin_block(int speed, int angle);
EXPORT_SYM int _create_get_raw_encoders(long *lenc, long *renc);
EXPORT_SYM void create_advance_led(int on) ;
EXPORT_SYM void create_play_led(int on) ;
EXPORT_SYM void create_power_led(int color, int brightness) ;
EXPORT_SYM void create_digital_output(int bits);
EXPORT_SYM void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0);
EXPORT_SYM void create_low_side_drivers(int pwm2, int pwm1, int pwm0);
EXPORT_SYM void create_load_song(int num);
EXPORT_SYM void create_play_song(int num);
EXPORT_SYM int create_read_block(char *data, int count);
EXPORT_SYM void create_write_byte(char byte);
EXPORT_SYM void create_clear_serial_buffer();

#ifdef __cplusplus
}
#endif

#endif
