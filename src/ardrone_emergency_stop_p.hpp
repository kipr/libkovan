#ifndef _ARDRONE_EMERGENCY_STOP_P_HPP_
#define _ARDRONE_EMERGENCY_STOP_P_HPP_

#include "kovan/thread.hpp"

class ARDrone;

namespace Private
{
	class ARDroneEmergencyStop : public Thread
	{
	public:
		ARDroneEmergencyStop(ARDrone *drone);
	
		virtual void run();
		void stop();
		
		void setEnabled(const bool enabled);
		bool isEnabled() const;
	
	private:
		ARDrone *m_drone;
		bool m_stop;
		bool m_enabled;
	};
}

#endif
