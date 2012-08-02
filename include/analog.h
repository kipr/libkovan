
#ifndef _ANALOG_H_
#define _ANALOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Gets the 10-bit analog value of a port.
 * \param[in] port A value between 0 and 7 specifying the sensor to read from.
 * \return The latest 10-bit value of the port (a value in the range 0 to 1023).
 * \see analog
 * \ingroup sensor
 */
int analog10(int port);

/*!
 * Gets the 8-bit analog value of a port.
 * \param[in] port A value between 0 and 7 specifying the sensor to read from.
 * \return The latest 8-bit value of the port (a value in the range 0 to 255).
 * \see analog10
 * \ingroup sensor
 */
int analog(int port);

void analog_set_pullup(int port, int pullup);
int analog_get_pullup(int port);

#ifdef __cplusplus
}
#endif

#endif
