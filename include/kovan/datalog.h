#ifndef _DATALOG_H_
#define _DATALOG_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYM void datalog_remove_category(const char *name);

EXPORT_SYM void datalog_append_string(const char *text, const char *category);
EXPORT_SYM void datalog_append_number(double number, const char *category);

EXPORT_SYM void datalog_remove(int index, const char *category);

EXPORT_SYM int datalog_write_plain(const char *path);
EXPORT_SYM int datalog_write_csv(const char *path);

EXPORT_SYM void datalog_clear();

#ifdef __cplusplus
}
#endif

#endif
