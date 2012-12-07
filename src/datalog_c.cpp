#include "kovan/datalog.h"
#include "kovan/datalog.hpp"

DataLog g_dataLog;

void datalog_remove_category(const char *name)
{
	g_dataLog.removeCategory(name);
}

void datalog_append_string(const char *text, const char *category)
{
	g_dataLog.category(category)->append(text);
}

void datalog_append_number(double number, const char *category)
{
	g_dataLog.category(category)->append(number);
}

void datalog_remove(int index, const char *category)
{
	g_dataLog.category(category)->remove(index);
}

int datalog_write_plain(const char *path)
{
	CsvWriter writer(path);
	return g_dataLog.write(&writer) ? 1 : 0;
}

int datalog_write_csv(const char *path)
{
	CsvWriter writer(path);
	return g_dataLog.write(&writer) ? 1 : 0;
}

void datalog_clear()
{
	g_dataLog.clear();
}