#include "EpollReactor.hpp"
#include <stdexcept>

EpollReactor::EventHandler::EventHandler(int p_fd)
	: m_fd(p_fd)
{
	if (p_fd < 0)
	{
		throw ::std::runtime_error("Invalid file descriptor");
	}
}

EpollReactor::EventHandler::~EventHandler()
{
}

int EpollReactor::EventHandler::getHandle() const
{
	return m_fd;
}

