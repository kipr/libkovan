#ifndef _BATTERY_P_H_
#define _BATTERY_P_H_

#include "common_p.hpp"

namespace Private
{
	class Battery : private Dev
	{
	public:
		typedef unsigned short level_t;
		
		level_t level();
		static Battery *instance();
		
	private:
		Battery(const char *dev);
	};
}

#endif
