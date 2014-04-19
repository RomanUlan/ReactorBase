#include "EpollReactor.hpp"
#include <stdexcept>
#include <sys/epoll.h>
#include <string.h>
#include <boost/scoped_array.hpp>

EpollReactor::EpollReactor()
	: m_epollFd(-1), m_maxEvents(100)
{
	m_epollFd = ::epoll_create(m_maxEvents);
	if (m_epollFd < 0)
	{
		throw ::std::runtime_error("Epoll create failed");
	}
}

EpollReactor::~EpollReactor()
{
	close(m_epollFd);
}

void EpollReactor::registerHandler(EventHandler::Ptr p_eventHandler)
{
	int fd = p_eventHandler->getHandle();
	int option = EPOLL_CTL_ADD;

	epoll_event e;
	::memset(&e, 0, sizeof(e));
	e.data.fd = fd;
	e.events	|= EPOLLIN	|  EPOLLRDHUP
				| EPOLLERR	|  EPOLLHUP;

	if (::epoll_ctl(m_epollFd, option, fd, &e) < 0)
	{
		throw ::std::runtime_error("Add handler to epoll failed");
	}

	m_handlers[fd] = p_eventHandler;
}

void EpollReactor::removeHandler(int p_fd)
{
	t_handlers::iterator i = m_handlers.find(p_fd);

	if (i == m_handlers.end())
	{
		throw ::std::runtime_error("Handler not found");
	}

	if (::epoll_ctl(m_epollFd, EPOLL_CTL_DEL, p_fd, 0) < 0)
	{
		m_handlers.erase(i);
		throw ::std::runtime_error("Cannot remove fd from epoll");
	}

	m_handlers.erase(i);
}

void EpollReactor::eventLoop() const
{
	while (1)
	{
		::boost::scoped_array<epoll_event> events(new epoll_event[m_maxEvents]);
		::memset(events.get(), 0, m_maxEvents * sizeof (epoll_event));
		int eventsLen = 0;

		eventsLen = ::epoll_wait(m_epollFd, events.get(), m_maxEvents, -1);
		if (eventsLen < 0)
		{
			throw ::std::runtime_error("Epoll wait failed");
		}

		for (int i = 0; i < eventsLen; ++i)
		{
			t_handlers::const_iterator ih = m_handlers.find(events[i].data.fd);
			if (ih != m_handlers.end())
			{
				ih->second->handleEvent(events[i].events);
			}
			else
			{
				throw ::std::runtime_error("Epoll returned event for \
											unfounded handler");
			}
		}
	} //while (1)
}

