#include "EchoResponderEH.hpp"
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdexcept>

EchoResponderEH::EchoResponderEH(int p_fd)
	: EventHandler(p_fd)
{
}

EchoResponderEH::~EchoResponderEH()
{
	::close(m_fd);
}

void EchoResponderEH::handleEvent(uint32_t p_event)
{
	if (p_event & EPOLLIN)
	{
		char msg[2048];
		::memset(msg, 0, 2048);
		int r = ::read(m_fd, msg, 2048);
		if (r < 0)
		{
			throw ::std::runtime_error("Cannot read");
		}

		int w = ::write(m_fd, msg, r);
		if (w < 0)
		{
			throw ::std::runtime_error("Cannot write");
		}
	}
	else
	{
		throw ::std::runtime_error("Bad event for for responder");
	}
}

