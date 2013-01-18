/*!
 * \file datalog.hpp
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup datalog Data Logging
 */

#ifndef _DATALOG_HPP_
#define _DATALOG_HPP_

#include <string>
#include <vector>
#include <map>

#include "export.h"

class DataLog;

/*!
 * \class Category
 * A category holds a collection of ordered and related items.
 * This is conceptually similar to a column in a spreadsheet.
 * \ingroup datalog
 */
class EXPORT_SYM Category
{
public:
	/*!
	 * Appends a string to this category.
	 */
	void append(const std::string& data);
	
	/*!
	 * Appends a number to this category.
	 */
	void append(const double& data);
	
	void remove(const unsigned int& index);
	
	/*!
	 * Gets a list of all entries contained in this category.
	 * \return The entries in this category, ordered.
	 */
	const std::vector<std::string>& entries() const;
	
	/*!
	 * \return The name associated with this category.
	 */
	const std::string& name() const;
	
	friend class DataLog;
private:
	Category(const std::string& name);
	
	std::string m_name;
	std::vector<std::string> m_entries;
};

/*!
 * \class DataLogWriter
 * Base class for data log writers.
 * \ingroup datalog
 */
class EXPORT_SYM DataLogWriter
{
public:
	virtual ~DataLogWriter();
	
	virtual bool write(const DataLog *dataLog) = 0;
};

/*!
 * \class CsvWriter
 * Writes a data log in the CSV spreadsheet format. This can be
 * imported into almost any spreadsheet program.
 * \ingroup datalog
 */
class EXPORT_SYM CsvWriter : public DataLogWriter
{
public:
	CsvWriter(const std::string& path);
	virtual bool write(const DataLog *dataLog);
	
private:
	std::string m_path;
};

/*!
 * \class PlainWriter
 * Writes a data log as plain text.
 * \ingroup datalog
 */
class EXPORT_SYM PlainWriter : public DataLogWriter
{
public:
	PlainWriter(const std::string& path);
	virtual bool write(const DataLog *dataLog);
private:
	unsigned int columnWidth(Category *category) const;
	
	std::string m_path;
};

/*!
 * \class DataLog
 * Holds a collection of related categories which can be written out to various formats.
 * This class is conceptually similar to a spreadsheet that holds multiple columns.
 * \ingroup datalog
 */
class EXPORT_SYM DataLog
{
public:
	~DataLog();
	/*!
	 * Returns the category instance associated with name.
	 * \param name The name of the category. This is considered the category's title.
	 * \return A new instance of Category if the category doesn't exist yet, or the existing instance if it does.
	 */
	Category *category(const std::string& name);
	
	/*!
	 * Removes a previously created category.
	 * \param name The name of the category to remove.
	 */
	void removeCategory(const std::string& name);
	
	/*!
	 * Get a list of all created categories.
	 * \return The currently created categories.
	 */
	std::vector<Category *> categories() const;
	
	/*!
	 * Writes out this Data Log with the given writer.
	 * \return true on success, false otherwise.
	 */
	bool write(DataLogWriter *writer) const;
	
	/*!
	 * Removes all categories from this Data Log
	 */
	void clear();
private:
	std::map<std::string, Category *> m_categories;
};

#endif
