#include "AcceptorEH.hpp"
#include "EchoResponderEH.hpp"
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdexcept>

AcceptorEH::AcceptorEH(int p_port, EpollReactor& p_reactor)
	: EventHandler(0), m_port(p_port), m_reactor(p_reactor)
{
	m_fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd < 0)
	{
		throw ::std::runtime_error("Cannot create acceptor socket");
	}

	int flags = fcntl(m_fd, F_GETFL, 0);
	fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);

	struct sockaddr_in addr;
	::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(p_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	::memcpy(&m_address, &addr, sizeof(addr));
	m_addressLength = sizeof addr;

	if (::bind(m_fd, &m_address, m_addressLength) < 0) {
		throw ::std::runtime_error("Cannot bind acceptor socket");
	}

	if (::listen(m_fd, 1) != 0) {
		throw ::std::runtime_error("Cannot listen on acceptor socket");
	}
}

AcceptorEH::~AcceptorEH()
{
	::close(m_fd);
}

void AcceptorEH::handleEvent(uint32_t p_event)
{
	if (p_event & EPOLLIN)
	{
		struct sockaddr addr;
		::memset(&addr, 0, sizeof(addr));
		socklen_t addrLen = sizeof(addr);
		int fd = ::accept(m_fd, &addr, &addrLen);
		if (fd < 0)
		{
			throw ::std::runtime_error("Cannot accept incoming connection");
		}

		EpollReactor::EventHandler::Ptr eh(new EchoResponderEH(fd));
		m_reactor.registerHandler(eh);
	}
	else
	{
		throw ::std::runtime_error("Bad event for acceptor");
	}
}

