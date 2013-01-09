#include "kovan/datalog.hpp"
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>

void Category::append(const std::string& data)
{
	m_entries.push_back(data);
}

void Category::append(const double& data)
{
	std::ostringstream sstream;
	sstream << data;
	append(sstream.str());
}

void Category::remove(const unsigned int& index)
{
	m_entries.erase(m_entries.begin() + index);
}

const std::vector<std::string>& Category::entries() const
{
	return m_entries;
}

const std::string& Category::name() const
{
	return m_name;
}

Category::Category(const std::string& name) : m_name(name) {}
DataLogWriter::~DataLogWriter() {}
CsvWriter::CsvWriter(const std::string& path) : m_path(path) {}

bool CsvWriter::write(const DataLog *dataLog)
{
	FILE *file = fopen(m_path.c_str(), "w");
	if(!file) return false;

	std::vector<Category *> categories = dataLog->categories();
	std::vector<Category *>::const_iterator it = categories.begin();
	
	for(it = categories.begin(); it != categories.end(); ++it) {
		if(it != categories.begin()) fprintf(file, ",");
		Category *category = *it;
		std::string entry = category->name();
		fprintf(file, "%s", entry.c_str());
	}
	fprintf(file, "\n");
	
	bool hit = true;
	for(unsigned int item = 0; hit; ++item) {
		hit = false;
		for(it = categories.begin(); it != categories.end(); ++it) {
			const std::vector<std::string>& entries = (*it)->entries();
			
			if(entries.size() <= item) continue;
			hit = true;
			
			if(it != categories.begin()) fprintf(file, ",");
			
			std::string entry = entries[item];
			fprintf(file, "%s", entry.c_str());
		}
		if(hit) fprintf(file, "\n");
	}
	
	fclose(file);
#ifndef WIN32
	sync();
#endif
	return true;
}


PlainWriter::PlainWriter(const std::string& path) : m_path(path) {}

bool PlainWriter::write(const DataLog *dataLog)
{
	FILE *file = fopen(m_path.c_str(), "w");
	if(!file) return false;
	
	std::vector<Category *> categories = dataLog->categories();
	std::vector<Category *>::const_iterator it = categories.begin();
	std::map<Category *, unsigned int> widths;
	for(; it != categories.end(); ++it) {
		Category *category = *it;
		widths[category] = columnWidth(category);
	}
	
	static const char *entryPad = "   ";
	
	for(it = categories.begin(); it != categories.end(); ++it) {
		Category *category = *it;
		std::string entry = category->name();
		unsigned int padding = widths[category] - entry.size();
		char *paddingStr = new char[padding + 1];
		memset(paddingStr, ' ', padding);
		paddingStr[padding] = 0;
		fprintf(file, "%s%s%s", paddingStr, entry.c_str(), entryPad);
		delete[] paddingStr;
	}
	fprintf(file, "\n");
	
	bool hit = true;
	for(unsigned int item = 0; hit; ++item) {
		hit = false;
		for(it = categories.begin(); it != categories.end(); ++it) {
			Category *category = *it;
			const std::vector<std::string>& entries = category->entries();
			std::vector<std::string>::const_iterator it = entries.begin();
			if(entries.size() > item) hit = true;
			else continue;
			std::string entry = entries[item];
			unsigned int padding = widths[category] - entry.size();
			char *paddingStr = new char[padding + 1];
			memset(paddingStr, ' ', padding);
			paddingStr[padding] = 0;
			fprintf(file, "%s%s%s", paddingStr, entry.c_str(), entryPad);
			delete[] paddingStr;
		}
		fprintf(file, "\n");
	}
	fclose(file);
#ifndef WIN32
	sync();
#endif
	return true;
}

unsigned int PlainWriter::columnWidth(Category *category) const
{
	const std::vector<std::string>& entries = category->entries();
	std::vector<std::string>::const_iterator it = entries.begin();
	unsigned int max = category->name().size();
	for(; it != entries.end(); ++it) {
		unsigned int size = (*it).size();
		max = max > size ? max : size;
	}
	return max;
}

DataLog::~DataLog()
{
	clear();
}

Category *DataLog::category(const std::string& name)
{
	std::map<std::string, Category *>::const_iterator it = m_categories.find(name);
	if(it == m_categories.end()) {
		Category *category = new Category(name);
		m_categories[name] = category;
		return category;
	}
	return it->second;
}

void DataLog::removeCategory(const std::string& name)
{
	m_categories.erase(name);
}

std::vector<Category *> DataLog::categories() const
{
	std::map<std::string, Category *>::const_iterator it = m_categories.begin();
	std::vector<Category *> ret;
	for(; it != m_categories.end(); ++it) ret.push_back(it->second);
	return ret;
}

bool DataLog::write(DataLogWriter *writer) const
{
	return writer->write(this);
}

void DataLog::clear()
{
	std::map<std::string, Category *>::const_iterator it = m_categories.begin();
	for(; it != m_categories.end(); ++it) delete it->second;
	m_categories.clear();
}