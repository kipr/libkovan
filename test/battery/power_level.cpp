#include <kovan/kovan.hpp>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	cout << "power level = " << Battery::powerLevel() << endl;
	return 0;
}