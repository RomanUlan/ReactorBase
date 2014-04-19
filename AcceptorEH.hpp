#ifndef ACCEPTOR_EH_HPP
#define ACCEPTOR_EH_HPP

#include "EpollReactor.hpp"
#include <sys/socket.h>

class AcceptorEH : public EpollReactor::EventHandler
{
	public:
		AcceptorEH(int, EpollReactor&);
		virtual ~AcceptorEH();

	public:
		virtual void handleEvent(uint32_t);

	private:
		int m_port;
		struct sockaddr m_address;
		socklen_t m_addressLength;
		EpollReactor& m_reactor;
};

#endif

