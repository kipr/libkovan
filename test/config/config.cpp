#include <kovan/kovan.hpp>
#include <cassert>

int main(int argc, char *argv[])
{
	Config config;
	config.setValue("bool", true);
	config.setValue("int", 21334);
	config.setValue("string", "babablsh?");
	config.setValue("awful string::", "babablsh? Hayouasd");
	
	Config merge;
	merge.setValue("test", "lololo");
	merge.setValue("test/subkey", "..adasdasd");
	config.beginGroup("group");
	config.addValues(merge);
	config.beginGroup("test");
	config.values().save("test_subkeys.conf");
	config.endGroup();
	config.endGroup();
	config.save("test.conf");
	
	Config *read = Config::load("test.conf");
	assert(read);
	assert(read->boolValue("bool") == true);
	assert(read->intValue("int") == 21334);
	assert(read->stringValue("awful string::").compare("babablsh? Hayouasd") == 0);
	delete read;
	
	
	
	return 0;
}