#ifndef _BATTERY_P_H_
#define _BATTERY_P_H_

namespace Private
{
	class Battery
	{
	public:
		typedef unsigned short level_t;
		
		level_t level();
		static Battery *instance();
		
	private:
		Battery();
	};
}

#endif
