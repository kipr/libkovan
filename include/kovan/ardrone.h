/**************************************************************************
 *  Copyright 2013 KISS Institute for Practical Robotics                  *
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

#ifndef _ARDRONE_H_
#define _ARDRONE_H_

/**
 * \file ardrone.h
 * \author Braden McDorman
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Establishes a connection between the drone and the Link.
 * This function must be called before any other ardrone functions.
 */
int drone_connect(void);

/*
 * \brief Disconnects from the drone.
 */
void drone_disconnect(void);

/**
 * \brief Calibrates the drone's accelerometers to understand what "flat" is.
 */
void drone_calibrate(void);

/**
 * \return The version of the currently connected drone. For example, an AR.Drone 1 will return
 * the integer 1. The value -1 is returned upon error.
 */ 
int get_drone_version(void);

/**
 * \brief Makes the drone takeoff and stabilize itself. This command will return immeadiately.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \post The drone should reach its normal operating height
 * \see drone_takeoff_block
 */
void drone_takeoff(void);

/**
 * \brief This function will be used to land the drone at its current position.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \post The drone should slowly descend to the ground from its current height.
 */
void drone_land(void);

/**
 * \brief retrieves the cached battery value
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return An integer representing the current battery level 
 */
int get_drone_battery(void);

/**
 * \brief Clears the accumulated absolute x, y, and z positions of the AR.Drone.
 * \pre drone_connect must have been previously called to esåtablish a connection to the drone.
 * \see get_drone_x
 * \see get_drone_y
 * \see get_drone_z
 */  
void drone_clear_position();

/**
 * \brief Retrieves the x value relative to the drones starting position.  Negative values indicate
 * the drone has moved to the left of it's starting position.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return x value away from the drones starting position in milimeters
 * \TODO verify it is in fact milimeters
 */
float get_drone_x(void);

/**
 * \brief Retrieves the y value relative to the drones starting position.  Negative values indicate
 * the drone has moved backwards from it's starting position.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return y value away from the drones starting position in milimeters
 **/
float get_drone_y(void);

/**
 * \brief Retrieves the y value relative to the drones starting position.  Negative values indicate
 * the drone has moved down from it's starting position.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return z value away from the drones starting position in milimeters
 */
float get_drone_z(void);

/**
 * \brief Retrieves the current velocity in the right or left direction.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return A float indicating the millimeters per second 
 */
float get_drone_x_velocity(void);

/**
 * \brief Retrieves the current velocity in the forward or backwards direction.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return A float indicating the velocity in millimeters per second 
 */
float get_drone_y_velocity(void);

/**
 * \brief Retrieves the current velocity in the upward or downwards direction.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return A float indicating the velocity in millimeters per second 
 */
float get_drone_z_velocity(void);

/**
 * \brief Retrieves the current pitch of the AR.Drone, in degrees.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return A float indicating the pitch of the AR.Drone in degrees.
 */
float get_drone_pitch(void);

/**
 * \brief Retrieves the current roll of the AR.Drone, in degrees.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return A float indicating the roll of the AR.Drone in degrees.
 */
float get_drone_roll(void);

/**
 * \brief Retrieves the current altitude of the AR.Drone, in meters.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return A float indicating the altitude of the AR.Drone in meters.
 */
float get_drone_altitude(void);

/**
 * \brief Retrieves the current rotation in the clockwise (positive) or counterclockwise (negative) direction.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \return A float indicating the degrees rotated from the original orientation
 */
float get_drone_yaw(void);

enum drone_camera
{
	FRONT_CAMERA,
	BOTTOM_CAMERA
};

/**
 * \brief Opens the AR.Drone's camera as the camera input device. You must use camera_close() once finished.
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 * \param camera FRONT_CAMERA for the horizontal camera, BOTTOM_CAMERA for the vertical camera.
 * \return 1 on success, 0 on failure
 */
int drone_camera_open(enum drone_camera camera);

/**
 * \brief Sets the Drone's MAC Address Pair to be the given string
 * \param macAddress A string representing the MAC Address to pair
 * \return 1 for success, 0 for failure
 * \see drone_pair
 */
int set_drone_mac_address(const char *const address);

/**
 * \brief Automatically detects the host MAC Address and pairs the drone with it
 * \return 1 for success, 0 for failure
 * \see set_drone_mac_address
 */
int drone_pair(void);

/**
 * Sets the SSID of the Drone to the given ssid.
 * \attention This setting will not take effect until the AR.Drone is restarted.
 * \return 1 for success, 0 for failure.
 */
int set_drone_ssid(const char *const ssid);
 
/**
 * \brief Tells the drone to move with the given parameters
 * \param enable A value indicating if movement is enabled. 0 - True 1 - False
 * \param x_tilt A value from zero to one indicating the percentage of maximum tilt in the left or right direction
 * negative values are left and positive values are right. Ex: -.5 means Half of the total tilt left
 * \param y_tilt A value from zero to one indicating the percentage of maximum tilt in the forward or backward direction
 * negative values are left and positive values are right. Ex: -.5 means Half of the total tilt backwards.
 * \param yaw_vel A value indicating the rotational velocity of the dronein milieters per second
 * \param z_vel A value indicating the change in altitude in milimeters per second
 */
void drone_move(float x_tilt, float y_tilt, float z_vel, float yaw_vel);

/**
 * \brief Tells the drone that it should stop moving and hover at its current location
 * \pre drone_connect must have been previously called to establish a connection to the drone.
 */
void drone_hover(void);

/**
 * When developing programs for the AR.Drone, it is often useful to be able to "emergency land".
 * This will turn the Link's side button into a dedicated AR.Drone "kill switch".
 * Note that using side_button in conjunction with this function may result in undefined behavior.
 * \param enabled 0 for off, 1 for on
 */
void set_drone_emergency_stop_enabled(int enabled);

/**
 * Gets the previously set emergency stop enabled flag.
 * \see set_drone_emergency_stop_enabled
 * \return 1 if emergency stop is enabled, 0 otherwise
 */ 
int get_drone_emergency_stop_enabled(void);

#ifdef __cplusplus
}
#endif

#endif
