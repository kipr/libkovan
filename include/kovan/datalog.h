#ifndef _DATALOG_H_
#define _DATALOG_H_

#include "export.h"

/*!
 * \file datalog.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup datalog Data Logging
 */


#ifdef __cplusplus
extern "C" {
#endif

/*!
* \ingroup datalog
*/
EXPORT_SYM void datalog_remove_category(const char *name);

/*!
 * \ingroup datalog
 */
EXPORT_SYM void datalog_append_string(const char *text, const char *category);

/*!
 * \ingroup datalog
 */
EXPORT_SYM void datalog_append_number(double number, const char *category);

/*!
 * \ingroup datalog
 */
EXPORT_SYM void datalog_remove(int index, const char *category);

/*!
 * \ingroup datalog
 */
EXPORT_SYM int datalog_write_plain(const char *path);

/*!
 * \ingroup datalog
 */
EXPORT_SYM int datalog_write_csv(const char *path);

/*!
 * \ingroup datalog
 */
EXPORT_SYM void datalog_clear();

#ifdef __cplusplus
}
#endif

#endif
