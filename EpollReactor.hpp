#ifndef EPOLL_REACTOR_HPP
#define EPOLL_REACTOR_HPP

#include <stdint.h>
#include <map>
#include <boost/shared_ptr.hpp>

class EpollReactor
{
public:
	class EventHandler
	{
	public:
		typedef ::boost::shared_ptr<EventHandler> Ptr;

	public:
		explicit EventHandler(int);
		virtual ~EventHandler();

	private:
		EventHandler(const EventHandler&);
		EventHandler& operator=(const EventHandler&);

	public:
		int getHandle() const;
		virtual void handleEvent(uint32_t) = 0;

	protected:
		int m_fd;
	}; //class EventHandler

public:
	EpollReactor();
	~EpollReactor();

private:
	EpollReactor(const EpollReactor&);
	EpollReactor& operator=(const EpollReactor&);

public:
	void registerHandler(EventHandler::Ptr);
	void removeHandler(int);
	void eventLoop() const;

private:
	typedef ::std::map<int, EventHandler::Ptr> t_handlers;
	int m_epollFd;
	int m_maxEvents;
	t_handlers m_handlers;
}; //class EpollReactor

#endif //EPOLL_REACTOR_HPP

