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

VH #include "kovan/create.h"
#include "kovan/create.hpp"
#include "nyi.h"

#include <climits>

VI int create_connect()
{
	while(!create_connect_once());
	return 0;
}

VI int create_connect_once()
{
	return Create::instance()->connect() ? 1 : 0;
}

VI void create_disconnect()
{
	Create::instance()->disconnect();
}

VI void create_passive()
{
	Create::instance()->setPassiveMode();
}

VI void create_safe()
{
	Create::instance()->setSafeMode();
}

VI void create_full()
{
	Create::instance()->setFullMode();
}

VI void create_start()
{
  THIS_IS_NYI
}

VI void create_spot()
{
  THIS_IS_NYI
}

VI void create_cover()
{
  THIS_IS_NYI
}

VI void create_demo(int d)
{
  THIS_IS_NYI
}

VI void create_cover_dock()
{
  THIS_IS_NYI
}

VI int get_create_mode()
{
	return Create::instance()->mode();
}

VI int get_create_lbump()
{
	return Create::instance()->bumpLeft()->value() ? 1 : 0;
}

VI int get_create_rbump()
{
	return Create::instance()->bumpRight()->value() ? 1 : 0;
}

VI int get_create_lwdrop()
{
	return Create::instance()->wheelDropLeft()->value() ? 1 : 0;
}

VI int get_create_cwdrop()
{
	return Create::instance()->wheelDropCaster()->value() ? 1 : 0;
}

VI int get_create_rwdrop()
{
	return Create::instance()->wheelDropRight()->value() ? 1 : 0;
}

VI int get_create_wall()
{
	return Create::instance()->wall()->value() ? 1 : 0;
}

VI int get_create_lcliff()
{
	return Create::instance()->cliffLeft()->value() ? 1 : 0;
}

VI int get_create_lfcliff()
{
	return Create::instance()->cliffFrontLeft()->value() ? 1 : 0;
}

VI int get_create_rfcliff()
{
	return Create::instance()->cliffFrontRight()->value() ? 1 : 0;
}

VI int get_create_rcliff()
{
	return Create::instance()->cliffRight()->value() ? 1 : 0;
}

VI int get_create_vwall()
{
	return Create::instance()->virtualWall()->value();
}

VI int get_create_overcurrents()
{
  THIS_IS_NYI
  return 0;
}

VI int get_create_infrared()
{
	return Create::instance()->ir()->value();
}

VI int get_create_advance_button()
{
	return Create::instance()->advanceButton()->value() ? 1 : 0;
}

VI int get_create_play_button()
{
	return Create::instance()->playButton()->value() ? 1 : 0;
}

VI int get_create_normalized_angle()
{
	return Create::instance()->angle()->value() % 360;
}

VI void set_create_normalized_angle(int angle)
{
	set_create_total_angle(angle);
}

VI int get_create_total_angle()
{
	return Create::instance()->angle()->value();
}

VI void set_create_total_angle(int angle)
{
	Create::instance()->setAngle(angle);
}

VI int get_create_distance()
{
	return Create::instance()->distance()->value();
}

VI void set_create_distance(int dist)
{
	Create::instance()->setDistance(dist);
}

VI int get_create_battery_charging_state()
{
	return Create::instance()->chargingState()->value();
}

VI int get_create_battery_voltage()
{
  THIS_IS_NYI
  return 0;
}

VI int get_create_battery_current()
{
  THIS_IS_NYI
  return 0;
}

VI int get_create_battery_temp()
{
	return Create::instance()->batteryTemperature()->value();
}

VI int get_create_battery_charge()
{
	return Create::instance()->batteryCharge()->value();
}

VI int get_create_battery_capacity()
{
	return Create::instance()->batteryCapacity()->value();
}

VI int get_create_wall_amt()
{
	return Create::instance()->wallSignal()->value();
}

VI int get_create_lcliff_amt()
{
	return Create::instance()->cliffLeftSignal()->value();
}

VI int get_create_lfcliff_amt()
{
	return Create::instance()->cliffFrontLeftSignal()->value();
}

VI int get_create_rfcliff_amt()
{
	return Create::instance()->cliffFrontRightSignal()->value();
}

VI int get_create_rcliff_amt()
{
	return Create::instance()->cliffRightSignal()->value();
}

VI int get_create_bay_DI()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_bay_AI()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_song_number()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_song_playing()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_number_of_stream_packets()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_requested_velocity()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_requested_radius()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_requested_right_velocity()
{
  THIS_IS_NYI
	return 0;
}

VI int get_create_requested_left_velocity()
{
  THIS_IS_NYI
	return 0;
}

VI void set_create_baud_rate(const enum BaudRate baudRate)
{
  Create::instance()->setDefaultBaudRate(static_cast<Create::BaudRate>(baudRate));
}

VI enum BaudRate get_create_baud_rate()
{
  return static_cast<BaudRate>(Create::instance()->defaultBaudRate());
}

VI void create_stop()
{
	Create::instance()->stop();
}

VI void create_drive(int speed, int radius)
{
	Create::instance()->drive(speed, radius);
}

VI void create_drive_straight(int speed)
{
	Create::instance()->driveStraight(speed);
}

VI void create_spin_CW(int speed)
{
	Create::instance()->spinClockwise(speed);
}

VI void create_spin_CCW(int speed)
{
	Create::instance()->spinCounterClockwise(speed);
}

VI void create_drive_direct(int l_speed, int r_speed)
{
	Create::instance()->driveDirect(l_speed, r_speed);
}

VI void create_spin_block(int speed, int angle)
{
	Create::instance()->turn(speed, angle);
}

VI int _create_get_raw_encoders(long *lenc, long *renc)
{
	return 0;
}

VI void create_advance_led(int on)
{
  THIS_IS_NYI
}

VI void create_play_led(int on)
{
  THIS_IS_NYI
}

VI void create_power_led(int color, int brightness)
{
  THIS_IS_NYI
}

VI void create_digital_output(int bits)
{
  THIS_IS_NYI
}

VI void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
  THIS_IS_NYI
}

VI void create_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
  THIS_IS_NYI
}

VI void create_load_song(int num)
{
  THIS_IS_NYI
}

VI void create_play_song(int num)
{
  THIS_IS_NYI
}

VI int create_read_block(char *data, int count)
{
	return Create::instance()->blockingRead(reinterpret_cast<unsigned char *>(data), count, UINT_MAX);
}

VI void create_write_byte(char byte)
{
	Create::instance()->write(static_cast<unsigned char>(byte));
}

VI void create_clear_serial_buffer()
{
	Create::instance()->flush();
}