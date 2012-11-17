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
		
		inline void enqueueCommand(const Command &command);
		
		bool flush();
		
		const State &currentState() const;
		
		static Kovan *instance();
	private:
		Kovan();
		
		KovanModule *m_module;
		State m_currentState;
		
		std::vector<Command> m_queue;
	};
}

#endif
