#include "kovan_p.hpp"

#include "kovan_module_p.hpp"

using namespace Private;

Kovan::~Kovan()
{
	delete m_module;
}

void Kovan::enqueueCommand(const Command &command)
{
	m_queue.push_back(command);
}

bool Kovan::flush()
{
	// TODO: This is a temporary requirement.
	Command stateCommand;
	stateCommand.type = StateCommandType;
	enqueueCommand(stateCommand);
	
	if(!m_module->send(m_queue)) return false;
	
	// TODO: This needs to be removed eventually.
	if(!m_module->recv(m_currentState)) return false;
	
	return true;
}

const State &Kovan::currentState() const
{
	return m_currentState;
}

Kovan *Kovan::instance()
{
	static Kovan s_instance;
	return &s_instance;
}

Kovan::Kovan()
	// TODO: This needs to be exposed via API (remote libkovan connection)
	: m_module(new KovanModule(inet_addr("127.0.0.1"), htons(5555)))
{
}

