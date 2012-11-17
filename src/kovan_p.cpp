#include "kovan_p.hpp"

#include "kovan_module_p.hpp"

#include <iostream> // FIXME: tmp

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
	std::vector<Command> sendQueue = m_queue;
	m_queue.clear();
	
	// TODO: This is a temporary requirement.
	Command stateCommand;
	stateCommand.type = StateCommandType;
	sendQueue.push_back(stateCommand);
	
	std::cout << "Sending queue to kovan module." << std::endl;
	
	if(!m_module->send(sendQueue)) return false;
	// TODO: This needs to be removed eventually.
	if(!m_module->recv(m_currentState)) return false;
	
	std::cout << "Queue successfully sent with State response." << std::endl;
	
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
	// Create the socket descriptor for communication
	if(!m_module->init()) {
		// TODO: Error Reporting?
		return;
	}
	
	// Bind out client to an address
	if(!m_module->bind(htonl(INADDR_ANY), htons(9999))) {
		// TODO: Error Reporting?
		return;
	}
}

