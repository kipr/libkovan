#include "kovan_p.hpp"

#include "kovan_module_p.hpp"
#include "kovan_regs_p.hpp"

#include <iostream> // FIXME: tmp

using namespace Private;

Kovan::~Kovan()
{
	delete m_module;
}

void Kovan::enqueueCommand(const Command &command, bool autoFlush)
{
	m_queue.push_back(command);
	
	// FIXME: This logic needs to be improved eventually
	if(autoFlush) autoUpdate();
}

void Kovan::setAutoFlush(const bool &autoFlush)
{
	m_autoFlush = autoFlush;
}

const bool &Kovan::autoFlush() const
{
	return m_autoFlush;
}

bool Kovan::flush()
{
	std::vector<Command> sendQueue = m_queue;
	m_queue.clear();
	
	// TODO: This is a temporary requirement.
	Command stateCommand;
	stateCommand.type = StateCommandType;
	sendQueue.push_back(stateCommand);
	
#ifdef LIBKOVAN_DEBUG
	std::cout << "Sending queue to kovan module." << std::endl;
#endif

	if(!m_module->send(sendQueue)) return false;
	// TODO: This needs to be removed eventually.
	if(!m_module->recv(m_currentState)) return false;

#ifdef LIBKOVAN_DEBUG	
	std::cout << "Queue successfully sent with State response." << std::endl;
	m_module->displayState(m_currentState);
#endif
	
	return true;
}

void Kovan::autoUpdate()
{
	if(m_autoFlush) flush();
}

State &Kovan::currentState()
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
	: m_module(new KovanModule(inet_addr("127.0.0.1"), htons(4628))),
	m_autoFlush(true)
{
	// Create the socket descriptor for communication
	if(!m_module->init()) {
		// TODO: Error Reporting?
		return;
	}
	
	// Bind out client to an address
	if(!m_module->bind(htonl(INADDR_ANY), htons(8374))) {
		// TODO: Error Reporting?
		return;
	}
}

