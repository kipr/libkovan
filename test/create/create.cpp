#include <kovan.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	Create *create = Create::instance();
	if(!create->connect()) {
		cout << "Create connection failed" << endl;
		return 1;
	}
	
	create->disconnect();
}