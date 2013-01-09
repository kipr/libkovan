#ifndef _KOVAN_P_HPP_
#define _KOVAN_P_HPP_

#include "kovan_command_p.hpp"

#include <vector>

namespace Private
{
	class KovanModule;
	
	class Kovan
	{
	public:
		~Kovan();
		
		void enqueueCommand(const Command &command, bool autoFlush = true);
		
		void setAutoFlush(const bool &autoFlush);
		const bool &autoFlush() const;
		bool flush();
		
		void autoUpdate();
		
		State &currentState();
		
		static Kovan *instance();
	private:
		Kovan();
		
		KovanModule *m_module;
		State m_currentState;
		
		bool m_autoFlush;
		std::vector<Command> m_queue;
	};
}

#endif
