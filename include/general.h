/*!
 * \file general.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup general General
 */

#ifndef _GENERAL_H_
#define _GENERAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Sets whether or not hardware output command publishing happens automatically.
 * \details publish() is, by default, called on your behalf by libkovan. When you turn automatic
 * publishing off, libkovan expects you to publish() changes manually.
 * \param[in] on 1 for automatic publishing, 0 for manual publishing
 * \note Any command involving hardware modification, such as motors or servos,
 * requires publish() to be called before changes will be written to the system.
 * \ingroup general
 */
void set_auto_publish(int on);

/*!
 * \brief Explictly publishes new data to the system
 * \details All hardware output commands are stored locally before being written to the system.
 * This allows for batch execution of commands with automatic publishing turned off.
 * \ingroup general
 */
void publish();

#ifdef __cplusplus
}
#endif

#endif
