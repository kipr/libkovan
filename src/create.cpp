#include "create.hpp"

Create *Create::instance()
{
	static Create s_create;
	return &s_create;
}

Create::Create() {}

Create::Create(const Create&) {}

Create& Create::operator=(const Create&) {}