#ifndef _DATALOG_H_
#define _DATALOG_H_

#ifdef __cplusplus
extern "C" {
#endif

void datalog_remove_category(const char *name);

void datalog_append_string(const char *text, const char *category);
void datalog_append_number(double number, const char *category);

void datalog_remove(int index, const char *category);

int datalog_write_plain(const char *path);
int datalog_write_csv(const char *path);

void datalog_clear();

#ifdef __cplusplus
}
#endif

#endif
